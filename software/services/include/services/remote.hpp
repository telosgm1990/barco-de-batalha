/*
 * Controle remoto pelo celular: a rede Wi-Fi do barco, a página do joystick e o
 * WebSocket por onde chegam os comandos e sai a telemetria.
 */
#pragma once

// Includes
#include <cstdint> // uint32_t, int8_t

// Enums/structs
namespace remote {

struct config_st {
    const char *ssid;
    const char *password;
    int channel;
    int8_t max_tx_power_qdbm; // 0 = padrão da placa
};

struct command_st {
    float x;              // -1 a 1, pro lado
    float y;              // -1 a 1, pra frente
    uint32_t received_ms; // quando chegou, no relógio da placa
    bool valid;           // já chegou algum comando desde que ligou
};

// Funções

// Suba a rede e o servidor da página
bool init(const config_st &config);

// Último comando que chegou do joystick
command_st get_command();

// Diga se tem um celular com a página aberta
bool is_connected();

// Mande a telemetria pra página, se tiver alguém ouvindo
void send_telemetry(const char *text);

} // namespace remote
