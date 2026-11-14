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
    // Guarde o pino e o divisor (tensão da bateria / tensão no pino)
    constexpr Battery(int pin, float divider_ratio)
        : _input(pin), _ratio(divider_ratio) {
    }

    // Ligue a leitura; false se o pino não for de ADC1
    bool begin();

    // Leia a tensão da bateria, em volts, com média de algumas amostras;
    // negativo se a leitura falhar
    float read_volts();

  private:
    platform::AnalogInput _input;
    float _ratio;
};

} // namespace components
