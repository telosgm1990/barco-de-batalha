/**
 * @file
 * Abstração: o controle remoto — de onde vêm o joystick e os ajustes, e pra
 * onde vai o retrato do barco. A camada de cima (o barco) depende disto;
 * quem implementa é WifiRemote.
 */
#pragma once

// Includes
#include <cstdint> // uint32_t

#include "services/drive.hpp" // drive_mode_e

// Enums/structs
namespace services {

/// O último joystick que chegou
struct command_st {
    float x;              ///< -1 a 1, pro lado
    float y;              ///< -1 a 1, pra frente
    uint32_t received_ms; ///< quando chegou, no relógio da placa
    bool valid;           ///< já chegou algum desde que ligou
};

/**
 * Ajustes pedidos pela página.
 *
 * Cada um tem o seu contador, que sobe a cada pedido novo — zero quer dizer
 * "ninguém pediu ainda". O barco compara com o último contador que viu: assim
 * percebe um pedido novo mesmo que o valor seja igual, e pedir um ajuste não
 * apaga o outro.
 */
struct settings_st {
    drive_mode_e mode; ///< modo pedido
    uint32_t mode_seq; ///< contador dos pedidos de modo
    float trim;        ///< ajuste fino, -1 a 1 (ver Drive::set_trim)
    uint32_t trim_seq; ///< contador dos pedidos de ajuste fino
};

/// Retrato do barco, que ele publica e a API entrega a quem pedir
struct status_st {
    float battery_volts; ///< tensão da bateria
    bool failsafe;       ///< motores parados por falta de joystick
    drive_mode_e mode;   ///< modo em uso
    float trim;          ///< ajuste fino em uso, -1 a 1
};

// Classes

/// O controle remoto, visto pelo barco
class Remote {
  public:
    /// Suba o controle (rede, página...); false se não deu
    virtual bool begin() = 0;

    /// Último joystick que chegou
    virtual command_st get_command() = 0;

    /// Últimos ajustes pedidos
    virtual settings_st get_settings() = 0;

    /// Diga se tem alguém com o controle aberto
    virtual bool is_connected() = 0;

    /// Publique o retrato do barco, pra quem perguntar
    virtual void publish_status(const status_st &status) = 0;

  protected:
    ~Remote() = default;
};

} // namespace services
