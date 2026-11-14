/*
 * Pino digital de saída.
 */
#pragma once

// Classes
namespace platform {

class OutputPin {
  public:
    // Configure o pino como saída, começando em nível baixo
    void init(int pin);

    // Escreva o nível do pino
    void write(bool high);

  private:
    int _pin = -1;
};

} // namespace platform
