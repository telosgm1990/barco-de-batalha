/*
 * Abstração: servidor da página, com WebSocket. Entrega uma página em "/" e
 * conversa com ela por mensagens de texto curtas em "/ws". A camada de cima
 * depende disto; quem implementa na placa é EspWebServer.
 */
#pragma once

// Includes
#include <cstddef> // size_t

// Classes
namespace platform {

// Quem quer ouvir as mensagens que chegam implementa isto
class TextListener {
  public:
    // Chegou uma mensagem de texto do cliente
    virtual void on_text(const char *text, size_t length, int client) = 0;

  protected:
    ~TextListener() = default;
};

class WebServer {
  public:
    // Suba o servidor com a página e quem ouve as mensagens
    virtual bool begin(
        const char *page,
        size_t page_length,
        TextListener &listener
    ) = 0;

    // Diga se o cliente ainda está com o WebSocket aberto
    virtual bool is_client_open(int client) = 0;

    // Mande uma mensagem de texto pro cliente, sem esperar
    virtual bool send_text(int client, const char *text) = 0;

  protected:
    ~WebServer() = default;
};

} // namespace platform
