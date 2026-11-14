/**
 * @file
 * Bateria lida por um divisor resistivo — implementa Battery. A ESP vê uma
 * fração da tensão, e aqui a conta volta pro valor da bateria. A entrada
 * analógica chega pronta, por referência, como a abstração da platform/.
 */
#pragma once

// Includes
#include "components/battery.hpp"    // Battery
#include "platform/analog_input.hpp" // AnalogInput

// Classes
namespace components {

/// Bateria lida pelo ADC através de um divisor resistivo
class DividerBattery final : public Battery {
  public:
    /// Receba a entrada e o divisor (tensão da bateria / tensão no pino)
    constexpr DividerBattery(platform::AnalogInput &input, float divider_ratio)
        : _input(input), _ratio(divider_ratio) {
    }

    /// Leia a tensão da bateria, com média de algumas amostras
    float read_volts() override;

  private:
    platform::AnalogInput &_input;
    float _ratio;
};

} // namespace components
