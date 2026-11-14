/*
 * Servidor HTTP com WebSocket: entrega a página do joystick e conversa com
 * ela por mensagens de texto curtas.
 */
#pragma once

// Includes
#include <cstddef> // size_t

// Enums/structs
namespace platform::http {

// Chamado a cada mensagem de texto que chega pelo WebSocket
using text_handler_t = void (*)(const char *text, size_t length, int client);

struct server_config_st {
    const char *page; // HTML da página, terminado em '\0'
    size_t page_length;
    text_handler_t on_text;
};

// Funções

// Suba o servidor: a página em "/" e o WebSocket em "/ws"
bool begin(const server_config_st &config);

// Diga se o cliente ainda está com o WebSocket aberto
bool is_client_open(int client);

// Mande uma mensagem de texto pro cliente, sem esperar
bool send_text(int client, const char *text);

} // namespace platform::http
