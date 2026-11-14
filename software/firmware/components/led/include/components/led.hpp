/*
 * Abstração: LED de status — aceso, apagado ou piscando num ritmo. A camada
 * de cima depende disto; quem implementa é DigitalLed.
 */
#pragma once

// Includes
#include <cstdint> // uint32_t

// Classes
namespace components {

class Led {
  public:
    // Comece apagado
    virtual void begin() = 0;

    // Atualize a piscada; chame sempre, com o relógio em ms
    virtual void update(uint32_t now_ms) = 0;

    // Acenda fixo (period_ms = 0) ou pisque com esse período
    virtual void set_blink(uint32_t period_ms) = 0;

    // Apague
    virtual void set_off() = 0;

  protected:
    ~Led() = default;
};

} // namespace components
