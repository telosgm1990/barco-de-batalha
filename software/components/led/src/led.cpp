// Includes
#include "components/led.hpp" // Led

// Funções públicas
namespace components {

void Led::init(int pin, bool active_low) {
    _active_low = active_low;
    _pin.init(pin);
    _write(false);
}

void Led::update(uint32_t now_ms) {
    if (!_enabled) {
        _write(false);
        return;
    }
    if (_period_ms == 0) {
        _write(true);
        return;
    }
    // Aceso na primeira metade do período, apagado na segunda
    _write((now_ms % _period_ms) < _period_ms / 2);
}

void Led::set_blink(uint32_t period_ms) {
    _enabled   = true;
    _period_ms = period_ms;
}

void Led::set_off() {
    _enabled = false;
}

void Led::_write(bool on) {
    _pin.write(on != _active_low);
}

} // namespace components
