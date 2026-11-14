// Includes
#include "platform/esp_web_server.hpp" // EspWebServer

#include <cstring> // strlen

#include "esp_http_server.h" // httpd_*

// Constantes/variáveis estáticas
static constexpr size_t MESSAGE_MAX = 64;

// Protótipos de funções estáticas
static esp_err_t _page_handler(httpd_req_t *request);
static esp_err_t _ws_handler(httpd_req_t *request);

// Funções públicas
namespace platform {

bool EspWebServer::begin(
    const char *page,
    size_t page_length,
    TextListener &listener
) {
    _page        = page;
    _page_length = page_length;
    _listener    = &listener;

    httpd_handle_t server        = nullptr;
    httpd_config_t server_config = HTTPD_DEFAULT_CONFIG();
    if (httpd_start(&server, &server_config) != ESP_OK) {
        return false;
    }
    _server = server;

    // Os dois caminhos chamam de volta com este objeto (user_ctx)
    httpd_uri_t page_uri = {};
    page_uri.uri         = "/";
    page_uri.method      = HTTP_GET;
    page_uri.handler     = _page_handler;
    page_uri.user_ctx    = this;
    httpd_register_uri_handler(server, &page_uri);

    httpd_uri_t ws_uri  = {};
    ws_uri.uri          = "/ws";
    ws_uri.method       = HTTP_GET;
    ws_uri.handler      = _ws_handler;
    ws_uri.user_ctx     = this;
    ws_uri.is_websocket = true;
    httpd_register_uri_handler(server, &ws_uri);
    return true;
}

bool EspWebServer::is_client_open(int client) {
    if (_server == nullptr || client < 0) {
        return false;
    }
    return httpd_ws_get_fd_info(static_cast<httpd_handle_t>(_server), client) ==
           HTTPD_WS_CLIENT_WEBSOCKET;
}

bool EspWebServer::send_text(int client, const char *text) {
    if (!is_client_open(client)) {
        return false;
    }
    httpd_ws_frame_t frame = {};
    frame.type             = HTTPD_WS_TYPE_TEXT;
    frame.payload = reinterpret_cast<uint8_t *>(const_cast<char *>(text));
    frame.len     = strlen(text);
    return httpd_ws_send_frame_async(
               static_cast<httpd_handle_t>(_server),
               client,
               &frame
           ) == ESP_OK;
}

const char *EspWebServer::get_page() const {
    return _page;
}

size_t EspWebServer::get_page_length() const {
    return _page_length;
}

TextListener *EspWebServer::get_listener() const {
    return _listener;
}

} // namespace platform

// Funções estáticas

// Entregue a página do joystick
static esp_err_t _page_handler(httpd_req_t *request) {
    auto *self = static_cast<platform::EspWebServer *>(request->user_ctx);
    httpd_resp_set_type(request, "text/html; charset=utf-8");
    return httpd_resp_send(request, self->get_page(), self->get_page_length());
}

// Receba uma mensagem do WebSocket e passe pra quem ouve
static esp_err_t _ws_handler(httpd_req_t *request) {
    if (request->method == HTTP_GET) {
        return ESP_OK; // aperto de mão do WebSocket
    }
    auto *self = static_cast<platform::EspWebServer *>(request->user_ctx);
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
    text[frame.len]                  = '\0';
    platform::TextListener *listener = self->get_listener();
    if (frame.type == HTTPD_WS_TYPE_TEXT && listener != nullptr) {
        listener->on_text(text, frame.len, httpd_req_to_sockfd(request));
    }
    return ESP_OK;
}
