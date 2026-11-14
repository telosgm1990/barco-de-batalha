/**
 * @file
 * Servidor HTTP + WebSocket do ESP-IDF — implementa WebServer.
 */
#pragma once

// Includes
#include <cstddef> // size_t

#include "platform/web_server.hpp" // WebServer, TextListener, RequestListener

// Classes
namespace platform {

/// Servidor web na placa, com o esp_http_server do ESP-IDF
class EspWebServer final : public WebServer {
  public:
    /// Suba o servidor: página em "/", WebSocket em "/ws", API em "/api/*"
    bool begin(
        const char *page,
        size_t page_length,
        TextListener &stream,
        RequestListener &api
    ) override;

    /// Diga se o cliente ainda está com o WebSocket aberto
    bool is_client_open(int client) override;

    /// @name Pro servidor do ESP-IDF, que chama de volta com este objeto
    /// @{
    const char *get_page() const;
    size_t get_page_length() const;
    TextListener *get_stream() const;
    RequestListener *get_api() const;
    /// @}

  private:
    void *_server         = nullptr;
    const char *_page     = nullptr;
    size_t _page_length   = 0;
    TextListener *_stream = nullptr;
    RequestListener *_api = nullptr;
};

} // namespace platform
