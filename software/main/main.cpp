/*
 * Ponto de entrada do firmware da DonaTelos: o bootstrap.
 *
 * As peças nascem com os pinos da placa (board.hpp); aqui elas vão pro
 * barco (init), o hardware e a rede ganham partida (begin) e o laço principal
 * roda pra sempre.
 */

// Includes
#include "app/boat.hpp"           // init, begin, update
#include "board.hpp"              // pinos e ajustes
#include "components/battery.hpp" // Battery
#include "components/led.hpp"     // Led
#include "components/motor.hpp"   // Motor
#include "platform/clock.hpp"     // millis, sleep_ms
#include "platform/pwm.hpp"       // begin_timer
#include "services/remote.hpp"    // config_st

#if __has_include("secrets.hpp")
#include "secrets.hpp" // senha do Wi-Fi
#else
#pragma message("secrets.hpp não existe: usando a senha de exemplo")
#include "secrets.example.hpp" // senha de exemplo
#endif

// Constantes/variáveis estáticas
// As peças, montadas com os pinos da placa — o construtor só guarda os
// números; o hardware liga no begin
constinit static components::Motor _left_motor(
    board::PIN_LEFT_IN1,
    board::PWM_LEFT_IN1,
    board::PIN_LEFT_IN2,
    board::PWM_LEFT_IN2,
    board::LEFT_MOTOR_REVERSED
);
constinit static components::Motor _right_motor(
    board::PIN_RIGHT_IN1,
    board::PWM_RIGHT_IN1,
    board::PIN_RIGHT_IN2,
    board::PWM_RIGHT_IN2,
    board::RIGHT_MOTOR_REVERSED
);
constinit static components::Battery _battery(
    board::PIN_BATTERY_SENSE,
    board::BATTERY_DIVIDER_RATIO
);
constinit static components::Led _led(board::PIN_LED, board::LED_ACTIVE_LOW);

// Ponto de entrada
extern "C" void app_main(void) {
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

    // Dê a partida: periféricos primeiro (motores parados), rede por último
    platform::PwmOutput::begin_timer(board::PWM_FREQUENCY_HZ);
    _left_motor.begin();
    _right_motor.begin();
    _battery.begin();
    _led.begin();
    boat::begin();

    // Rode o laço principal
    while (true) {
        boat::update(platform::clock::millis());
        platform::clock::sleep_ms(board::LOOP_PERIOD_MS);
    }
}
