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
    // Configure o timer compartilhado (uma vez, antes dos canais)
    static void init_timer(uint32_t frequency_hz);

    // Ligue um canal a um pino, começando parado
    void init(int pin, int channel);

    // Escreva o ciclo de trabalho, de 0 (desligado) a 1 (sempre ligado)
    void write(float duty);

  private:
    int _channel = -1;
};

} // namespace platform
