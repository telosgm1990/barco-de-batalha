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
    // Prepare o LED; active_low quando ele acende com o pino em zero
    void init(int pin, bool active_low);

    // Atualize a piscada; chame sempre, com o relógio em ms
    void update(uint32_t now_ms);

    // Acenda fixo (period_ms = 0) ou pisque com esse período
    void set_blink(uint32_t period_ms);

    // Apague
    void set_off();

  private:
    void _write(bool on);

    platform::OutputPin _pin;
    bool _active_low    = false;
    bool _enabled       = false;
    uint32_t _period_ms = 0;
};

} // namespace components
