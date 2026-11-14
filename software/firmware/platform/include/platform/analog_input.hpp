/**
 * @file
 * Abstração: entrada analógica. A camada de cima depende disto; quem
 * implementa na placa é EspAnalogInput.
 */
#pragma once

// Includes
#include <cstdint> // int32_t

// Classes
namespace platform {

/// Uma entrada analógica, em milivolts
class AnalogInput {
  public:
    /// Leia a tensão no pino, em milivolts; negativo se falhar
    virtual int32_t read_mv() = 0;

  protected:
    ~AnalogInput() = default;
};

} // namespace platform
