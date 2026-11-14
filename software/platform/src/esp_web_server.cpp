// Includes
#include "platform/esp_web_server.hpp" // EspWebServer

#include <cstring> // memcpy, strcspn

#include "esp_http_server.h" // httpd_*

// Constantes/variáveis estáticas
static constexpr size_t MESSAGE_MAX  = 64;  // mensagem do WebSocket
static constexpr size_t API_PATH_MAX = 32;  // caminho da API, sem a query
static constexpr size_t BODY_MAX     = 128; // corpo de um pedido da API
static constexpr size_t RESPONSE_MAX = 160; // resposta da API

// Protótipos de funções estáticas

/// Entregue a página do joystick
static esp_err_t _page_handler(httpd_req_t *request);

/// Receba uma mensagem do WebSocket e passe pra quem ouve
static esp_err_t _ws_handler(httpd_req_t *request);

/// Leia o pedido da API, passe pra quem atende e mande a resposta
static esp_err_t _api_handler(httpd_req_t *request);

/**
 * Leia o corpo inteiro do pedido.
 *
 * @return false se não coube em size (com o '\0') ou se a conexão falhou
 */
static bool _read_body(httpd_req_t *request, char *body, size_t size);

/// Linha de status HTTP ("200 OK"...) de um código
static const char *_status_line(int status);

// Funções públicas
namespace platform {

bool EspWebServer::begin(
    const char *page,
    size_t page_length,
    TextListener &stream,
    RequestListener &api
) {
    _page        = page;
    _page_length = page_length;
    _stream      = &stream;
    _api         = &api;

    httpd_handle_t server        = nullptr;
    httpd_config_t server_config = HTTPD_DEFAULT_CONFIG();
    // "/api/*" pega todos os recursos da API
    server_config.uri_match_fn = httpd_uri_match_wildcard;
    // A página pergunta o estado toda hora: feche a conexão mais velha se
    // acabarem os sockets
    server_config.lru_purge_enable = true;
    if (httpd_start(&server, &server_config) != ESP_OK) {
        return false;
    }
    _server = server;

    // Todos os caminhos chamam de volta com este objeto (user_ctx)
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

    // Um registro por método; método que não está aqui ganha 405 do servidor
    httpd_uri_t api_uri = {};
    api_uri.uri         = "/api/*";
    api_uri.handler     = _api_handler;
    api_uri.user_ctx    = this;
    api_uri.method      = HTTP_GET;
    httpd_register_uri_handler(server, &api_uri);
    api_uri.method = HTTP_PATCH;
    httpd_register_uri_handler(server, &api_uri);
    return true;
}

bool EspWebServer::is_client_open(int client) {
    if (_server == nullptr || client < 0) {
        return false;
    }
    return httpd_ws_get_fd_info(static_cast<httpd_handle_t>(_server), client) ==
           HTTPD_WS_CLIENT_WEBSOCKET;
}

const char *EspWebServer::get_page() const {
    return _page;
}

size_t EspWebServer::get_page_length() const {
    return _page_length;
}

TextListener *EspWebServer::get_stream() const {
    return _stream;
}

RequestListener *EspWebServer::get_api() const {
    return _api;
}

} // namespace platform

// Funções estáticas

static esp_err_t _page_handler(httpd_req_t *request) {
    auto *self = static_cast<platform::EspWebServer *>(request->user_ctx);
    httpd_resp_set_type(request, "text/html; charset=utf-8");
    return httpd_resp_send(request, self->get_page(), self->get_page_length());
}

static esp_err_t _ws_handler(httpd_req_t *request) {
    if (request->method == HTTP_GET) {
        return ESP_OK; // aperto de mão do WebSocket
    }
    auto *self = static_cast<platform::EspWebServer *>(request->user_ctx);
    // Primeiro só o tamanho; depois a mensagem, se couber
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
    text[frame.len]                = '\0';
    platform::TextListener *stream = self->get_stream();
    if (frame.type == HTTPD_WS_TYPE_TEXT && stream != nullptr) {
        stream->on_text(text, frame.len, httpd_req_to_sockfd(request));
    }
    return ESP_OK;
}

static esp_err_t _api_handler(httpd_req_t *request) {
    auto *self = static_cast<platform::EspWebServer *>(request->user_ctx);

    // Caminho sem a query ("/api/status?x=1" vira "/api/status")
    char path[API_PATH_MAX] = "";
    size_t path_length      = strcspn(request->uri, "?");
    if (path_length >= sizeof(path)) {
        path_length = 0; // grande demais: vira "" e dá 404
    }
    memcpy(path, request->uri, path_length);
    path[path_length] = '\0';

    char body[BODY_MAX] = "";
    if (!_read_body(request, body, sizeof(body))) {
        httpd_resp_set_status(request, "413 Content Too Large");
        return httpd_resp_send(request, nullptr, 0);
    }

    platform::http_method_e method = request->method == HTTP_PATCH
                                         ? platform::http_method_e::PATCH
                                         : platform::http_method_e::GET;
    char response[RESPONSE_MAX]    = "";
    int status =
        self->get_api()
            ->on_request(method, path, body, response, sizeof(response));

    httpd_resp_set_status(request, _status_line(status));
    if (status == platform::HTTP_NO_CONTENT) {
        return httpd_resp_send(request, nullptr, 0);
    }
    httpd_resp_set_type(request, "application/json");
    return httpd_resp_send(request, response, HTTPD_RESP_USE_STRLEN);
}

static bool _read_body(httpd_req_t *request, char *body, size_t size) {
    if (request->content_len >= size) {
        return false;
    }
    size_t received = 0;
    while (received < request->content_len) {
        int got = httpd_req_recv(
            request,
            body + received,
            request->content_len - received
        );
        if (got == HTTPD_SOCK_ERR_TIMEOUT) {
            continue; // devagar, mas vem: tente de novo
        }
        if (got <= 0) {
            return false;
        }
        received += static_cast<size_t>(got);
    }
    body[received] = '\0';
    return true;
}

static const char *_status_line(int status) {
    switch (status) {
    case platform::HTTP_OK:
        return "200 OK";
    case platform::HTTP_NO_CONTENT:
        return "204 No Content";
    case platform::HTTP_BAD_REQUEST:
        return "400 Bad Request";
    case platform::HTTP_NOT_FOUND:
        return "404 Not Found";
    case platform::HTTP_METHOD_NOT_ALLOWED:
        return "405 Method Not Allowed";
    default:
        return "500 Internal Server Error";
    }
}
