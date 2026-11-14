// Includes
#include "platform/esp_digital_output.hpp" // EspDigitalOutput

#include "driver/gpio.h" // gpio_*

// Funções públicas
namespace platform {

void EspDigitalOutput::begin() {
    if (_pin < 0) {
        return;
    }
    gpio_num_t num = static_cast<gpio_num_t>(_pin);
    gpio_reset_pin(num);
    gpio_set_direction(num, GPIO_MODE_OUTPUT);
    gpio_set_level(num, 0);
}

void EspDigitalOutput::write(bool high) {
    if (_pin < 0) {
        return;
    }
    gpio_set_level(static_cast<gpio_num_t>(_pin), high ? 1 : 0);
}

} // namespace platform
