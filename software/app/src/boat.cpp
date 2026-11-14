// Includes
#include "app/boat.hpp" // boat

#include "app/failsafe.hpp"      // Failsafe
#include "platform/log.hpp"      // info
#include "services/drive.hpp"    // update, stop, get_output
#include "services/protocol.hpp" // format_telemetry
#include "services/remote.hpp"   // get_command, is_connected

// Constantes/variáveis estáticas
static constexpr const char *TAG = "boat";

// Piscadas do LED: esperando o celular, e failsafe com o celular conectado
static constexpr uint32_t LED_WAITING_MS  = 1000;
static constexpr uint32_t LED_FAILSAFE_MS = 200;

static boat::parts_st _parts   = {};
static boat::tuning_st _tuning = {};
static app::Failsafe _failsafe{0}; // o tempo de verdade chega no init
static uint32_t _last_update_ms    = 0;
static uint32_t _last_telemetry_ms = 0;
static bool _was_tripped           = true;

// Protótipos de funções estáticas
static void _update_led(uint32_t now_ms, bool connected, bool tripped);
static void _send_telemetry(bool tripped);

// Funções públicas
namespace boat {

void init(
    const parts_st &parts,
    const tuning_st &tuning,
    const remote::config_st &remote_config
) {
    _parts    = parts;
    _tuning   = tuning;
    _failsafe = app::Failsafe(tuning.failsafe_timeout_ms);
    drive::init(tuning.slew_per_second, tuning.deadzone);
    remote::init(remote_config);
}

void begin() {
    remote::begin();
    platform::log::info(TAG, "DonaTelos acordou. Devagar e sempre.");
}

void update(uint32_t now_ms) {
    float dt        = (now_ms - _last_update_ms) / 1000.0f;
    _last_update_ms = now_ms;

    // Vigie o failsafe antes de qualquer coisa
    remote::command_st command = remote::get_command();
    _failsafe.update(now_ms, command.received_ms, command.valid);
    bool tripped = _failsafe.is_tripped();
    if (tripped != _was_tripped) {
        platform::log::info(
            TAG,
            tripped ? "failsafe armado: motores parados" : "comando de volta"
        );
        _was_tripped = tripped;
    }

    // Pare na hora se armou; senão, ande a tração rumo ao joystick
    if (tripped) {
        drive::stop();
    }
    else {
        drive::update(dt, command.x, command.y);
    }
    drive::wheels_st wheels = drive::get_output();
    _parts.left->set_speed(wheels.left);
    _parts.right->set_speed(wheels.right);

    bool connected = remote::is_connected();
    _update_led(now_ms, connected, tripped);
    if (connected &&
        now_ms - _last_telemetry_ms >= _tuning.telemetry_period_ms) {
        _last_telemetry_ms = now_ms;
        _send_telemetry(tripped);
    }
}

} // namespace boat

// Funções estáticas

// LED: pisca devagar esperando, aceso com controle, pisca rápido no failsafe
static void _update_led(uint32_t now_ms, bool connected, bool tripped) {
    if (!connected) {
        _parts.led->set_blink(LED_WAITING_MS);
    }
    else if (tripped) {
        _parts.led->set_blink(LED_FAILSAFE_MS);
    }
    else {
        _parts.led->set_blink(0);
    }
    _parts.led->update(now_ms);
}

static void _send_telemetry(bool tripped) {
    char text[48];
    protocol::format_telemetry(
        text,
        sizeof(text),
        _parts.battery->read_volts(),
        tripped
    );
    remote::send_telemetry(text);
}
