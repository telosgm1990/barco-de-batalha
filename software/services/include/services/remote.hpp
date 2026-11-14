/*
 * Abstração: o controle remoto — de onde vêm os comandos do joystick e pra
 * onde vai a telemetria. A camada de cima (o barco) depende disto; quem
 * implementa é WifiRemote.
 */
#pragma once

// Includes
#include <cstdint> // uint32_t

#include "services/drive.hpp" // drive_mode_e

// Enums/structs
namespace services {

struct command_st {
    float x;              // -1 a 1, pro lado
    float y;              // -1 a 1, pra frente
    uint32_t received_ms; // quando chegou o joystick, no relógio da placa
    bool valid;           // já chegou algum joystick desde que ligou
};

// Ajustes pedidos pela página. Cada um tem o seu contador, que sobe a cada
// pedido novo — zero quer dizer "ninguém pediu ainda". Assim, pedir um não
// apaga o outro.
struct settings_st {
    drive_mode_e mode;
    uint32_t mode_seq;
    float trim; // ajuste fino, -1 a 1 (ver Drive::set_trim)
    uint32_t trim_seq;
};

// Classes

class Remote {
  public:
    // Suba o controle (rede, página...); false se não deu
    virtual bool begin() = 0;

    // Último comando que chegou do joystick
    virtual command_st get_command() = 0;

    // Últimos ajustes pedidos pela página
    virtual settings_st get_settings() = 0;

    // Diga se tem alguém com o controle aberto
    virtual bool is_connected() = 0;

    // Mande a telemetria, se tiver alguém ouvindo
    virtual void send_telemetry(const char *text) = 0;

  protected:
    ~Remote() = default;
};

} // namespace services
