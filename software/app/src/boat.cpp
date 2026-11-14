// Includes
#include "app/boat.hpp" // Boat

#include "services/protocol.hpp" // format_telemetry

// Constantes/variáveis estáticas
static constexpr const char *TAG = "boat";

// Piscadas do LED: esperando o celular, e failsafe com o celular conectado
static constexpr uint32_t LED_WAITING_MS  = 1000;
static constexpr uint32_t LED_FAILSAFE_MS = 200;

// Funções públicas
namespace app {

void Boat::begin() {
    _parts.remote.begin();
    _parts.logger.info(TAG, "DonaTelos acordou. Devagar e sempre.");
}

void Boat::update(uint32_t now_ms) {
    float dt        = (now_ms - _last_update_ms) / 1000.0f;
    _last_update_ms = now_ms;

    // Vigie o failsafe antes de qualquer coisa
    services::command_st command = _parts.remote.get_command();
    _apply_settings(command);
    _failsafe.update(now_ms, command.received_ms, command.valid);
    bool tripped = _failsafe.is_tripped();
    if (tripped != _was_tripped) {
        _parts.logger.info(
            TAG,
            tripped ? "failsafe armado: motores parados" : "comando de volta"
        );
        _was_tripped = tripped;
    }

    // Pare na hora se armou; senão, ande a tração rumo ao joystick
    if (tripped) {
        _drive.stop();
    }
    else {
        _drive.update(dt, command.x, command.y);
    }
    services::wheels_st wheels = _drive.get_output();
    _parts.left.set_speed(wheels.left);
    _parts.right.set_speed(wheels.right);

    bool connected = _parts.remote.is_connected();
    _update_led(now_ms, connected, tripped);
    if (connected &&
        now_ms - _last_telemetry_ms >= _tuning.telemetry_period_ms) {
        _last_telemetry_ms = now_ms;
        _send_telemetry(tripped);
    }
}

services::drive_mode_e Boat::get_mode() const {
    return _mode;
}

// A página pediu ajuste novo: troque o modo (o perfil da tração)
void Boat::_apply_settings(const services::command_st &command) {
    if (command.settings_seq == _settings_seq) {
        return;
    }
    _settings_seq = command.settings_seq;
    if (command.mode != _mode) {
        _mode = command.mode;
        _drive.set_profile(_tuning.profiles[services::mode_index(_mode)]);
        _parts.logger
            .info(TAG, "modo %s", services::protocol::mode_name(_mode));
    }
}

// LED: pisca devagar esperando, aceso com controle, pisca rápido no failsafe
void Boat::_update_led(uint32_t now_ms, bool connected, bool tripped) {
    if (!connected) {
        _parts.led.set_blink(LED_WAITING_MS);
    }
    else if (tripped) {
        _parts.led.set_blink(LED_FAILSAFE_MS);
    }
    else {
        _parts.led.set_blink(0);
    }
    _parts.led.update(now_ms);
}

void Boat::_send_telemetry(bool tripped) {
    char text[80];
    services::protocol::telemetry_st telemetry =
        {_parts.battery.read_volts(), tripped, _mode};
    services::protocol::format_telemetry(text, sizeof(text), telemetry);
    _parts.remote.send_telemetry(text);
}

} // namespace app
