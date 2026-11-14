/**
 * @file
 * Abstração: saída PWM. A camada de cima depende disto; quem implementa na
 * placa é EspPwmOutput.
 */
#pragma once

// Classes
namespace platform {

/// Uma saída PWM
class PwmOutput {
  public:
    /// Escreva o ciclo de trabalho, de 0 (desligado) a 1 (sempre ligado)
    virtual void write(float duty) = 0;

  protected:
    ~PwmOutput() = default;
};

} // namespace platform
