/*
 * Saída PWM — o LEDC do ESP32-C3, um canal por pino.
 */
#pragma once

// Includes
#include <cstdint> // uint32_t

// Classes
namespace platform {

class PwmOutput {
  public:
    // Guarde o pino e o canal — sem mexer no hardware
    constexpr PwmOutput(int pin, int channel) : _pin(pin), _channel(channel) {
    }

    // Ligue o timer compartilhado (uma vez, antes de qualquer canal)
    static void begin_timer(uint32_t frequency_hz);

    // Ligue o canal ao pino, começando parado
    void begin();

    // Escreva o ciclo de trabalho, de 0 (desligado) a 1 (sempre ligado)
    void write(float duty);

  private:
    int _pin;
    int _channel;
    bool _started = false;
};

} // namespace platform
