/*
 * Servidor HTTP + WebSocket do ESP-IDF — implementa WebServer.
 */
#pragma once

// Includes
#include <cstddef> // size_t

#include "platform/web_server.hpp" // WebServer, TextListener

// Classes
namespace platform {

class EspWebServer final : public WebServer {
  public:
    // Suba o servidor com a página e quem ouve as mensagens
    bool begin(
        const char *page,
        size_t page_length,
        TextListener &listener
    ) override;

    // Diga se o cliente ainda está com o WebSocket aberto
    bool is_client_open(int client) override;

    // Mande uma mensagem de texto pro cliente, sem esperar
    bool send_text(int client, const char *text) override;

    // Usadas pelo servidor do ESP-IDF, que chama de volta com este objeto
    const char *get_page() const;
    size_t get_page_length() const;
    TextListener *get_listener() const;

  private:
    void *_server           = nullptr;
    const char *_page       = nullptr;
    size_t _page_length     = 0;
    TextListener *_listener = nullptr;
};

} // namespace platform
