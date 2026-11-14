// Includes
#include "components/motor.hpp" // Motor

#include "common/math.hpp" // clamp

// Funções públicas
namespace components {

void Motor::begin() {
    _in1.begin();
    _in2.begin();
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
