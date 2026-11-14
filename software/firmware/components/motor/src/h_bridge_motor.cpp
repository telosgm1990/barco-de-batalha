// Includes
#include "components/h_bridge_motor.hpp" // HBridgeMotor

#include "common/math.hpp" // clamp

// Funções públicas
namespace components {

void HBridgeMotor::begin() {
    set_speed(0.0f);
}

void HBridgeMotor::set_speed(float speed) {
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
