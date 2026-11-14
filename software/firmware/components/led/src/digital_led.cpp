// Includes
#include "components/digital_led.hpp" // DigitalLed

// Funções públicas
namespace components {

void DigitalLed::begin() {
    _write(false);
}

void DigitalLed::update(uint32_t now_ms) {
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

void DigitalLed::set_blink(uint32_t period_ms) {
    _enabled   = true;
    _period_ms = period_ms;
}

void DigitalLed::set_off() {
    _enabled = false;
}

void DigitalLed::_write(bool on) {
    _output.write(on != _active_low);
}

} // namespace components
