/*
 * Motor DC numa ponte H de duas entradas.
 *
 * Pra frente, PWM na entrada 1 e a 2 em zero; pra trás, o contrário; parado,
 * as duas em zero (o motor fica solto).
 */
#pragma once

// Includes
#include "platform/pwm.hpp" // PwmOutput

// Classes
namespace components {

class Motor {
  public:
    // Ligue o motor às duas entradas da ponte H; reversed inverte o sentido
    void init(
        int pin_in1,
        int channel_in1,
        int pin_in2,
        int channel_in2,
        bool reversed
    );

    // Velocidade de -1 (ré cheia) a 1 (frente cheia)
    void set_speed(float speed);

  private:
    platform::PwmOutput _in1;
    platform::PwmOutput _in2;
    bool _reversed = false;
};

} // namespace components
