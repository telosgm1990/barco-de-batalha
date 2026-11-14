/*
 * Saída digital num pino do ESP32-C3 — implementa DigitalOutput.
 */
#pragma once

// Includes
#include "platform/digital_output.hpp" // DigitalOutput

// Classes
namespace platform {

class EspDigitalOutput final : public DigitalOutput {
  public:
    // Guarde o número do pino — sem mexer no hardware
    constexpr explicit EspDigitalOutput(int pin) : _pin(pin) {
    }

    // Configure o pino como saída, começando em nível baixo
    void begin();

    // Escreva o nível do pino
    void write(bool high) override;

  private:
    int _pin;
};

} // namespace platform
