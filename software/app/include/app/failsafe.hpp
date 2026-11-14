/*
 * Failsafe: perdeu o celular, os motores param.
 *
 * A página manda comando o tempo todo, mesmo parada. Se passar do tempo
 * limite sem chegar nada — ou se nunca chegou nada desde que ligou —, o
 * failsafe arma. Barco sem controle andando sozinho é pior que barco parado.
 */
#pragma once

// Includes
#include <cstdint> // uint32_t

// Classes
namespace app {

class Failsafe {
  public:
    // Guarde o tempo limite sem comando; nasce armado
    constexpr explicit Failsafe(uint32_t timeout_ms) : _timeout_ms(timeout_ms) {
    }

    // Confira o relógio contra o último comando que chegou
    void update(uint32_t now_ms, uint32_t last_command_ms, bool has_command);

    // Diga se está armado (motores têm que parar)
    bool is_tripped() const;

  private:
    uint32_t _timeout_ms;
    bool _tripped = true;
};

} // namespace app
