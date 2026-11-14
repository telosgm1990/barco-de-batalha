/*
 * Abstração: o controle remoto — de onde vêm os comandos do joystick e pra
 * onde vai a telemetria. A camada de cima (o barco) depende disto; quem
 * implementa é WifiRemote.
 */
#pragma once

// Includes
#include <cstdint> // uint32_t

// Enums/structs
namespace services {

struct command_st {
    float x;              // -1 a 1, pro lado
    float y;              // -1 a 1, pra frente
    uint32_t received_ms; // quando chegou, no relógio da placa
    bool valid;           // já chegou algum comando desde que ligou
};

// Classes

class Remote {
  public:
    // Suba o controle (rede, página...); false se não deu
    virtual bool begin() = 0;

    // Último comando que chegou do joystick
    virtual command_st get_command() = 0;

    // Diga se tem alguém com o controle aberto
    virtual bool is_connected() = 0;

    // Mande a telemetria, se tiver alguém ouvindo
    virtual void send_telemetry(const char *text) = 0;

  protected:
    ~Remote() = default;
};

} // namespace services
