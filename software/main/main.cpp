/*
 * Ponto de entrada do firmware da DonaTelos: o bootstrap.
 *
 * Monta as peças com os pinos da placa (board.hpp), entrega pro barco e roda
 * o laço principal pra sempre.
 */

// Includes
#include "app/boat.hpp"           // init, update
#include "board.hpp"              // pinos e ajustes
#include "components/battery.hpp" // Battery
#include "components/led.hpp"     // Led
#include "components/motor.hpp"   // Motor
#include "platform/clock.hpp"     // millis, sleep_ms
#include "platform/pwm.hpp"       // init_timer
#include "services/remote.hpp"    // config_st

#if __has_include("secrets.hpp")
#include "secrets.hpp" // senha do Wi-Fi
#else
#pragma message("secrets.hpp não existe: usando a senha de exemplo")
#include "secrets.example.hpp" // senha de exemplo
#endif

// Constantes/variáveis estáticas
static components::Motor _left_motor;
static components::Motor _right_motor;
static components::Battery _battery;
static components::Led _led;

// Ponto de entrada
extern "C" void app_main(void) {
    // Monte as peças com os pinos da placa
    platform::PwmOutput::init_timer(board::PWM_FREQUENCY_HZ);
    _left_motor.init(
        board::PIN_LEFT_IN1,
        board::PWM_LEFT_IN1,
        board::PIN_LEFT_IN2,
        board::PWM_LEFT_IN2,
        board::LEFT_MOTOR_REVERSED
    );
    _right_motor.init(
        board::PIN_RIGHT_IN1,
        board::PWM_RIGHT_IN1,
        board::PIN_RIGHT_IN2,
        board::PWM_RIGHT_IN2,
        board::RIGHT_MOTOR_REVERSED
    );
    _battery.init(board::PIN_BATTERY_SENSE, board::BATTERY_DIVIDER_RATIO);
    _led.init(board::PIN_LED, board::LED_ACTIVE_LOW);

    // Entregue tudo pro barco
    boat::parts_st parts   = {&_left_motor, &_right_motor, &_battery, &_led};
    boat::tuning_st tuning = {
        board::FAILSAFE_TIMEOUT_MS,
        board::DRIVE_SLEW_PER_SECOND,
        board::JOYSTICK_DEADZONE,
        board::TELEMETRY_PERIOD_MS
    };
    remote::config_st remote_config = {
        secrets::WIFI_SSID,
        secrets::WIFI_PASSWORD,
        board::WIFI_CHANNEL,
        board::WIFI_MAX_TX_POWER_QDBM
    };
    boat::init(parts, tuning, remote_config);

    // Rode o laço principal
    while (true) {
        boat::update(platform::clock::millis());
        platform::clock::sleep_ms(board::LOOP_PERIOD_MS);
    }
}
