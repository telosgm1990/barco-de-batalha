/**
 * @file
 * Abstração: saída liga/desliga. A camada de cima depende disto; quem
 * implementa na placa é EspDigitalOutput.
 */
#pragma once

// Classes
namespace platform {

/// Uma saída liga/desliga
class DigitalOutput {
  public:
    /// Escreva o nível: true é alto
    virtual void write(bool high) = 0;

  protected:
    /// Ninguém apaga uma peça pela abstração: os objetos são estáticos
    ~DigitalOutput() = default;
};

} // namespace platform
