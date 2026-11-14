/*
 * LED de status: aceso, apagado ou piscando num ritmo.
 */
#pragma once

// Includes
#include <cstdint> // uint32_t

#include "platform/gpio.hpp" // OutputPin

// Classes
namespace components {

class Led {
  public:
    // Guarde o pino; active_low quando ele acende com o pino em zero
    constexpr Led(int pin, bool active_low)
        : _pin(pin), _active_low(active_low) {
    }

    // Ligue o pino, com o LED apagado
    void begin();

    // Atualize a piscada; chame sempre, com o relógio em ms
    void update(uint32_t now_ms);

    // Acenda fixo (period_ms = 0) ou pisque com esse período
    void set_blink(uint32_t period_ms);

    // Apague
    void set_off();

  private:
    void _write(bool on);

    platform::OutputPin _pin;
    bool _active_low;
    bool _enabled       = false;
    uint32_t _period_ms = 0;
};

} // namespace components
