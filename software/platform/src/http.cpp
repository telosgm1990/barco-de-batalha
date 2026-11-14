// Includes
#include "platform/http.hpp" // http

#include <cstring> // strlen

#include "esp_http_server.h" // httpd_*

// Constantes/variáveis estáticas
static constexpr size_t MESSAGE_MAX = 64;

static httpd_handle_t _server                   = nullptr;
static platform::http::server_config_st _config = {};

// Protótipos de funções estáticas
static esp_err_t _page_handler(httpd_req_t *request);
static esp_err_t _ws_handler(httpd_req_t *request);

// Funções públicas
namespace platform::http {

bool begin(const server_config_st &config) {
    _config                      = config;
    httpd_config_t server_config = HTTPD_DEFAULT_CONFIG();
    if (httpd_start(&_server, &server_config) != ESP_OK) {
        return false;
    }

    httpd_uri_t page = {};
    page.uri         = "/";
    page.method      = HTTP_GET;
    page.handler     = _page_handler;
    httpd_register_uri_handler(_server, &page);

    httpd_uri_t ws  = {};
    ws.uri          = "/ws";
    ws.method       = HTTP_GET;
    ws.handler      = _ws_handler;
    ws.is_websocket = true;
    httpd_register_uri_handler(_server, &ws);
    return true;
}

bool is_client_open(int client) {
    if (_server == nullptr || client < 0) {
        return false;
    }
    return httpd_ws_get_fd_info(_server, client) == HTTPD_WS_CLIENT_WEBSOCKET;
}

bool send_text(int client, const char *text) {
    if (!is_client_open(client)) {
        return false;
    }
    httpd_ws_frame_t frame = {};
    frame.type             = HTTPD_WS_TYPE_TEXT;
    frame.payload = reinterpret_cast<uint8_t *>(const_cast<char *>(text));
    frame.len     = strlen(text);
    return httpd_ws_send_frame_async(_server, client, &frame) == ESP_OK;
}

} // namespace platform::http

// Funções estáticas

// Entregue a página do joystick
static esp_err_t _page_handler(httpd_req_t *request) {
    httpd_resp_set_type(request, "text/html; charset=utf-8");
    return httpd_resp_send(request, _config.page, _config.page_length);
}

// Receba uma mensagem do WebSocket e passe pra frente
static esp_err_t _ws_handler(httpd_req_t *request) {
    if (request->method == HTTP_GET) {
        return ESP_OK; // aperto de mão do WebSocket
    }
    httpd_ws_frame_t frame = {};
    frame.type             = HTTPD_WS_TYPE_TEXT;
    if (httpd_ws_recv_frame(request, &frame, 0) != ESP_OK) {
        return ESP_FAIL;
    }
    if (frame.len == 0 || frame.len >= MESSAGE_MAX) {
        return ESP_OK; // mensagem vazia ou grande demais: ignore
    }
    char text[MESSAGE_MAX];
    frame.payload = reinterpret_cast<uint8_t *>(text);
    if (httpd_ws_recv_frame(request, &frame, frame.len) != ESP_OK) {
        return ESP_FAIL;
    }
    text[frame.len] = '\0';
    if (frame.type == HTTPD_WS_TYPE_TEXT && _config.on_text != nullptr) {
        _config.on_text(text, frame.len, httpd_req_to_sockfd(request));
    }
    return ESP_OK;
}
