// Includes
#include "services/drive.hpp" // Drive

#include <cmath> // std::fabs

#include "common/math.hpp" // approach, clamp, deadzone

// Funções públicas
namespace services {

void Drive::update(float dt_seconds, float x, float y) {
    x = common::deadzone(common::clamp(x, -1.0f, 1.0f), _deadzone);
    y = common::deadzone(common::clamp(y, -1.0f, 1.0f), _deadzone);
    x = shape(x, _profile.expo);
    y = shape(y, _profile.expo);
    wheels_st target = mix(x, y);
    // Limite do modo e ajuste fino: tire força da roda que puxa mais
    float left_gain  = _trim < 0.0f ? 1.0f + _trim : 1.0f;
    float right_gain = _trim > 0.0f ? 1.0f - _trim : 1.0f;
    target.left *= _profile.max_output * left_gain;
    target.right *= _profile.max_output * right_gain;
    // Chegue no alvo aos poucos, no máximo a rampa por segundo
    float step    = _profile.slew_per_second * dt_seconds;
    _output.left  = common::approach(_output.left, target.left, step);
    _output.right = common::approach(_output.right, target.right, step);
}

wheels_st Drive::get_output() const {
    return _output;
}

void Drive::set_profile(const drive_profile_st &profile) {
    _profile = profile;
}

void Drive::set_trim(float trim) {
    _trim = common::clamp(trim, -1.0f, 1.0f);
}

void Drive::stop() {
    _output = {0.0f, 0.0f};
}

wheels_st Drive::mix(float x, float y) {
    wheels_st wheels = {y + x, y - x};
    // Se uma roda passou de 1, encolha as duas na mesma proporção
    float biggest = std::fabs(wheels.left);
    if (std::fabs(wheels.right) > biggest) {
        biggest = std::fabs(wheels.right);
    }
    if (biggest > 1.0f) {
        wheels.left /= biggest;
        wheels.right /= biggest;
    }
    return wheels;
}

float Drive::shape(float value, float expo) {
    expo = common::clamp(expo, 0.0f, 1.0f);
    return (1.0f - expo) * value + expo * value * value * value;
}

} // namespace services
