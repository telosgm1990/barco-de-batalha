// Includes
#include "app/boat.hpp" // Boat

#include <cmath> // std::lround

#include "common/math.hpp"       // clamp
#include "services/protocol.hpp" // format_telemetry, mode_name

// Constantes/variáveis estáticas
static constexpr const char *TAG = "boat";

// Nome do ajuste fino na memória da placa; guardado em por cento
static constexpr const char *TRIM_KEY = "trim";
static constexpr float TRIM_SCALE     = 100.0f;

// Piscadas do LED: esperando o celular, e failsafe com o celular conectado
static constexpr uint32_t LED_WAITING_MS  = 1000;
static constexpr uint32_t LED_FAILSAFE_MS = 200;

// Funções públicas
namespace app {

void Boat::begin() {
    // Volte com o ajuste fino da última vez; sem nada guardado, fica reto
    int32_t saved = 0;
    if (_parts.store.read_int(TRIM_KEY, &saved)) {
        _set_trim(saved / TRIM_SCALE);
    }
    _parts.remote.begin();
    _parts.logger.info(TAG, "DonaTelos acordou. Devagar e sempre.");
}

void Boat::update(uint32_t now_ms) {
    float dt        = (now_ms - _last_update_ms) / 1000.0f;
    _last_update_ms = now_ms;

    // Vigie o failsafe antes de qualquer coisa
    _apply_settings();
    services::command_st command = _parts.remote.get_command();
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

float Boat::get_trim() const {
    return _trim;
}

// A página pediu ajuste novo: troque o modo (o perfil da tração) ou o ajuste
// fino — esse fica guardado pra próxima vez que o barco ligar
void Boat::_apply_settings() {
    services::settings_st settings = _parts.remote.get_settings();
    if (settings.mode_seq != _mode_seq) {
        _mode_seq = settings.mode_seq;
        if (settings.mode != _mode) {
            _mode = settings.mode;
            _drive.set_profile(_tuning.profiles[services::mode_index(_mode)]);
            _parts.logger
                .info(TAG, "modo %s", services::protocol::mode_name(_mode));
        }
    }
    if (settings.trim_seq != _trim_seq) {
        _trim_seq       = settings.trim_seq;
        float last_trim = _trim;
        _set_trim(settings.trim);
        if (_trim != last_trim) {
            long percent = std::lround(_trim * TRIM_SCALE);
            if (!_parts.store.write_int(TRIM_KEY, percent)) {
                _parts.logger.warn(TAG, "não consegui guardar o ajuste fino");
            }
            _parts.logger.info(TAG, "ajuste fino %+ld%%", percent);
        }
    }
}

// Ajuste fino, limitado ao máximo do tuning
void Boat::_set_trim(float trim) {
    _trim = common::clamp(trim, -_tuning.max_trim, _tuning.max_trim);
    _drive.set_trim(_trim);
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
        {_parts.battery.read_volts(), tripped, _mode, _trim};
    services::protocol::format_telemetry(text, sizeof(text), telemetry);
    _parts.remote.send_telemetry(text);
}

} // namespace app
