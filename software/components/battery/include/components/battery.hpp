/*
 * Bateria lida por um divisor resistivo: a ESP vê uma fração da tensão, e
 * aqui a conta volta pro valor da bateria.
 */
#pragma once

// Includes
#include "platform/adc.hpp" // AnalogInput

// Classes
namespace components {

class Battery {
  public:
    // Prepare a leitura; divider_ratio = tensão da bateria / tensão no pino
    bool init(int pin, float divider_ratio);

    // Leia a tensão da bateria, em volts, com média de algumas amostras;
    // negativo se a leitura falhar
    float read_volts();

  private:
    platform::AnalogInput _input;
    float _ratio = 1.0f;
};

} // namespace components
