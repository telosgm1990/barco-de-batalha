// Includes
#include "components/motor.hpp" // Motor

#include "common/math.hpp" // clamp

// Funções públicas
namespace components {

void Motor::init(
    int pin_in1,
    int channel_in1,
    int pin_in2,
    int channel_in2,
    bool reversed
) {
    _reversed = reversed;
    _in1.init(pin_in1, channel_in1);
    _in2.init(pin_in2, channel_in2);
}

void Motor::set_speed(float speed) {
    speed = common::clamp(speed, -1.0f, 1.0f);
    if (_reversed) {
        speed = -speed;
    }
    // Uma entrada com PWM, a outra em zero, conforme o sentido
    if (speed >= 0.0f) {
        _in2.write(0.0f);
        _in1.write(speed);
    }
    else {
        _in1.write(0.0f);
        _in2.write(-speed);
    }
}

} // namespace components
