/*
 * Pino digital de saída.
 */
#pragma once

// Classes
namespace platform {

class OutputPin {
  public:
    // Guarde o número do pino — sem mexer no hardware
    constexpr explicit OutputPin(int pin) : _pin(pin) {
    }

    // Configure o pino como saída, começando em nível baixo
    void begin();

    // Escreva o nível do pino
    void write(bool high);

  private:
    int _pin;
};

} // namespace platform
