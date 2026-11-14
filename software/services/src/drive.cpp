// Includes
#include "services/drive.hpp" // drive

#include <cmath> // std::fabs

#include "common/math.hpp" // approach, clamp, deadzone

// Constantes/variáveis estáticas
static float _slew_per_second   = 1.0f;
static float _deadzone          = 0.0f;
static drive::wheels_st _output = {0.0f, 0.0f};

// Funções públicas
namespace drive {

void init(float slew_per_second, float deadzone) {
    _slew_per_second = slew_per_second;
    _deadzone        = deadzone;
    _output          = {0.0f, 0.0f};
}

void update(float dt_seconds, float x, float y) {
    x = common::deadzone(common::clamp(x, -1.0f, 1.0f), _deadzone);
    y = common::deadzone(common::clamp(y, -1.0f, 1.0f), _deadzone);
    wheels_st target = mix(x, y);
    // Chegue no alvo aos poucos, no máximo a rampa por segundo
    float step    = _slew_per_second * dt_seconds;
    _output.left  = common::approach(_output.left, target.left, step);
    _output.right = common::approach(_output.right, target.right, step);
}

wheels_st get_output() {
    return _output;
}

void stop() {
    _output = {0.0f, 0.0f};
}

wheels_st mix(float x, float y) {
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

} // namespace drive
