// Includes
#include "app/boat.hpp" // Boat

#include <cmath> // std::lround

#include "common/math.hpp"       // clamp
#include "services/protocol.hpp" // mode_name

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
    bool changed    = _apply_settings();

    // Vigie o failsafe antes de mexer nos motores
    services::command_st command = _parts.remote.get_command();
    _failsafe.update(now_ms, command.received_ms, command.valid);
    bool tripped = _failsafe.is_tripped();
    if (tripped != _was_tripped) {
        _parts.logger.info(
            TAG,
            tripped ? "failsafe armado: motores parados" : "comando de volta"
        );
        _was_tripped = tripped;
        changed      = true;
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

    _update_led(now_ms, _parts.remote.is_connected(), tripped);

    // Publique de tempos em tempos, ou já, se algo mudou
    if (changed || now_ms - _last_status_ms >= _tuning.status_period_ms) {
        _last_status_ms = now_ms;
        _publish_status(now_ms, tripped);
    }
}

services::drive_mode_e Boat::get_mode() const {
    return _mode;
}

float Boat::get_trim() const {
    return _trim;
}

// Troque o modo (o perfil da tração) ou o ajuste fino — esse fica guardado
// pra próxima vez que o barco ligar
bool Boat::_apply_settings() {
    services::settings_st settings = _parts.remote.get_settings();
    bool changed                   = false;
    if (settings.mode_seq != _mode_seq) {
        _mode_seq = settings.mode_seq;
        if (settings.mode != _mode) {
            _mode = settings.mode;
            _drive.set_profile(_tuning.profiles[services::mode_index(_mode)]);
            _parts.logger
                .info(TAG, "modo %s", services::protocol::mode_name(_mode));
            changed = true;
        }
    }
    if (settings.trim_seq != _trim_seq) {
        _trim_seq       = settings.trim_seq;
        float last_trim = _trim;
        _set_trim(settings.trim);
        if (_trim != last_trim) {
            int32_t percent =
                static_cast<int32_t>(std::lround(_trim * TRIM_SCALE));
            if (!_parts.store.write_int(TRIM_KEY, percent)) {
                _parts.logger.warn(TAG, "não consegui guardar o ajuste fino");
            }
            _parts.logger
                .info(TAG, "ajuste fino %+ld%%", static_cast<long>(percent));
            changed = true;
        }
    }
    return changed;
}

void Boat::_set_trim(float trim) {
    _trim = common::clamp(trim, -_tuning.max_trim, _tuning.max_trim);
    _drive.set_trim(_trim);
}

// Pisca devagar esperando, aceso com controle, pisca rápido no failsafe
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

void Boat::_publish_status(uint32_t now_ms, bool tripped) {
    float volts = _parts.battery.read_volts();
    _battery_watch.update(now_ms, volts);
    bool low = _battery_watch.is_low();
    if (low != _was_low) {
        if (low) {
            _parts.logger.warn(TAG, "bateria baixa: %.2f V", double(volts));
        }
        else {
            _parts.logger.info(TAG, "bateria ok: %.2f V", double(volts));
        }
        _was_low = low;
    }
    _parts.remote.publish_status({volts, tripped, low, _mode, _trim});
}

} // namespace app
