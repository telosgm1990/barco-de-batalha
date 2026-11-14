/*
 * LED ligado numa saída digital — implementa Led. A saída chega pronta, por
 * referência, como a abstração da platform/.
 */
#pragma once

// Includes
#include <cstdint> // uint32_t

#include "components/led.hpp"          // Led
#include "platform/digital_output.hpp" // DigitalOutput

// Classes
namespace components {

class DigitalLed final : public Led {
  public:
    // Receba a saída; active_low quando o LED acende com o pino em zero
    constexpr DigitalLed(platform::DigitalOutput &output, bool active_low)
        : _output(output), _active_low(active_low) {
    }

    // Comece apagado
    void begin() override;

    // Atualize a piscada; chame sempre, com o relógio em ms
    void update(uint32_t now_ms) override;

    // Acenda fixo (period_ms = 0) ou pisque com esse período
    void set_blink(uint32_t period_ms) override;

    // Apague
    void set_off() override;

  private:
    void _write(bool on);

    platform::DigitalOutput &_output;
    bool _active_low;
    bool _enabled       = false;
    uint32_t _period_ms = 0;
};

} // namespace components
