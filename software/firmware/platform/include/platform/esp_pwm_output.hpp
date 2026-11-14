/**
 * @file
 * Saída PWM do ESP32-C3 (o LEDC), um canal por pino — implementa PwmOutput.
 */
#pragma once

// Includes
#include <cstdint> // uint32_t

#include "platform/pwm_output.hpp" // PwmOutput

// Classes
namespace platform {

/// Um canal do LEDC ligado a um pino
class EspPwmOutput final : public PwmOutput {
  public:
    /// Guarde o pino e o canal — sem mexer no hardware
    constexpr EspPwmOutput(int pin, int channel)
        : _pin(pin), _channel(channel) {
    }

    /// Ligue o timer compartilhado (uma vez, antes de qualquer canal)
    static void begin_timer(uint32_t frequency_hz);

    /// Ligue o canal ao pino, começando parado
    void begin();

    /// Escreva o ciclo de trabalho, de 0 (desligado) a 1 (sempre ligado)
    void write(float duty) override;

  private:
    int _pin;
    int _channel;
    bool _started = false;
};

} // namespace platform
