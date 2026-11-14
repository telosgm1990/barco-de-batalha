/*
 * Abstração: relógio e espera. A camada de cima depende disto; quem
 * implementa na placa é EspClock.
 */
#pragma once

// Includes
#include <cstdint> // uint32_t

// Classes
namespace platform {

class Clock {
  public:
    // Milissegundos desde que a placa ligou
    virtual uint32_t millis() = 0;

    // Durma a tarefa atual por ms milissegundos
    virtual void sleep_ms(uint32_t ms) = 0;

  protected:
    ~Clock() = default;
};

} // namespace platform
