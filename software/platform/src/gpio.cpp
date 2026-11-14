// Includes
#include "platform/gpio.hpp" // OutputPin

#include "driver/gpio.h" // gpio_*

// Funções públicas
namespace platform {

void OutputPin::init(int pin) {
    _pin           = pin;
    gpio_num_t num = static_cast<gpio_num_t>(pin);
    gpio_reset_pin(num);
    gpio_set_direction(num, GPIO_MODE_OUTPUT);
    gpio_set_level(num, 0);
}

void OutputPin::write(bool high) {
    if (_pin < 0) {
        return;
    }
    gpio_set_level(static_cast<gpio_num_t>(_pin), high ? 1 : 0);
}

} // namespace platform
