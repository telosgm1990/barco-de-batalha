/*
 * Motor DC numa ponte H de duas entradas (TC1508A, MX1508 e afins) —
 * implementa Motor.
 *
 * Pra frente, PWM na entrada 1 e a 2 em zero; pra trás, o contrário; parado,
 * as duas em zero (o motor fica solto). As duas saídas PWM chegam prontas, por
 * referência, como a abstração da platform/.
 */
#pragma once

// Includes
#include "components/motor.hpp"    // Motor
#include "platform/pwm_output.hpp" // PwmOutput

// Classes
namespace components {

class HBridgeMotor final : public Motor {
  public:
    // Receba as duas entradas da ponte H; reversed inverte o sentido
    constexpr HBridgeMotor(
        platform::PwmOutput &in1,
        platform::PwmOutput &in2,
        bool reversed
    )
        : _in1(in1), _in2(in2), _reversed(reversed) {
    }

    // Comece parado
    void begin() override;

    // Velocidade de -1 (ré cheia) a 1 (frente cheia)
    void set_speed(float speed) override;

  private:
    platform::PwmOutput &_in1;
    platform::PwmOutput &_in2;
    bool _reversed;
};

} // namespace components
