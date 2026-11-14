/**
 * @file
 * Ponto de entrada do firmware da DonaTelos: o bootstrap.
 *
 * É o único lugar que conhece as classes concretas. Aqui nasce cada peça da
 * placa (as Esp* da platform/) com o pino do board.hpp, e cada camada recebe,
 * por referência, a abstração da camada de baixo: o motor recebe as saídas
 * PWM, o controle remoto recebe a rede e o servidor, o barco recebe tudo.
 * Depois o hardware ganha partida (begin) e o laço principal roda pra sempre.
 */

// Includes
#include "app/boat.hpp"                     // Boat
#include "board.hpp"                        // pinos e ajustes
#include "components/digital_led.hpp"       // DigitalLed
#include "components/divider_battery.hpp"   // DividerBattery
#include "components/h_bridge_motor.hpp"    // HBridgeMotor
#include "platform/esp_access_point.hpp"    // EspAccessPoint
#include "platform/esp_analog_input.hpp"    // EspAnalogInput
#include "platform/esp_clock.hpp"           // EspClock
#include "platform/esp_digital_output.hpp"  // EspDigitalOutput
#include "platform/esp_key_value_store.hpp" // EspKeyValueStore
#include "platform/esp_logger.hpp"          // EspLogger
#include "platform/esp_pwm_output.hpp"      // EspPwmOutput
#include "platform/esp_web_server.hpp"      // EspWebServer
#include "services/page.hpp"                // page_start, page_end
#include "services/wifi_remote.hpp"         // WifiRemote

#if __has_include("secrets.hpp")
#include "secrets.hpp" // senha do Wi-Fi
#else
#pragma message("secrets.hpp não existe: usando a senha de exemplo")
#include "secrets.example.hpp" // senha de exemplo
#endif

// Constantes/variáveis estáticas

// platform/: as peças da placa, cada uma só com o pino; ligam no begin
constinit static platform::EspPwmOutput _left_in1(
    board::PIN_LEFT_IN1,
    board::PWM_LEFT_IN1
);
constinit static platform::EspPwmOutput _left_in2(
    board::PIN_LEFT_IN2,
    board::PWM_LEFT_IN2
);
constinit static platform::EspPwmOutput _right_in1(
    board::PIN_RIGHT_IN1,
    board::PWM_RIGHT_IN1
);
constinit static platform::EspPwmOutput _right_in2(
    board::PIN_RIGHT_IN2,
    board::PWM_RIGHT_IN2
);
constinit static platform::EspAnalogInput _battery_input(
    board::PIN_BATTERY_SENSE
);
constinit static platform::EspDigitalOutput _status_output(
    board::PIN_LED_STATUS
);
constinit static platform::EspDigitalOutput _alert_output(board::PIN_LED_ALERT);
constinit static platform::EspClock _clock;
constinit static platform::EspLogger _logger;
constinit static platform::EspAccessPoint _access_point(
    {secrets::WIFI_SSID,
     secrets::WIFI_PASSWORD,
     board::WIFI_CHANNEL,
     board::WIFI_MAX_TX_POWER_QDBM}
);
constinit static platform::EspWebServer _web_server;
constinit static platform::EspKeyValueStore _store(board::STORE_SPACE);

// components/: recebem as abstrações da platform/
constinit static components::HBridgeMotor _left_motor(
    _left_in1,
    _left_in2,
    board::LEFT_MOTOR_REVERSED
);
constinit static components::HBridgeMotor _right_motor(
    _right_in1,
    _right_in2,
    board::RIGHT_MOTOR_REVERSED
);
constinit static components::DividerBattery _battery(
    _battery_input,
    board::BATTERY_DIVIDER_RATIO
);
constinit static components::DigitalLed _status_led(
    _status_output,
    board::LED_ACTIVE_LOW
);
constinit static components::DigitalLed _alert_led(
    _alert_output,
    board::LED_ACTIVE_LOW
);

// services/: o controle remoto recebe a rede, o servidor, o relógio e o log
constinit static services::WifiRemote _remote(
    _access_point,
    _web_server,
    _clock,
    _logger,
    services::page_start,
    services::page_end
);

// app/: o barco recebe tudo
constinit static app::Boat _boat(
    {_left_motor,
     _right_motor,
     _battery,
     _status_led,
     _alert_led,
     _remote,
     _logger,
     _store},
    {board::FAILSAFE_TIMEOUT_MS,
     board::JOYSTICK_DEADZONE,
     board::STATUS_PERIOD_MS,
     {board::PROFILE_ECONOMY,
      board::PROFILE_PRECISE,
      board::PROFILE_FULL,
      board::PROFILE_WAR},
     board::INITIAL_MODE,
     board::MAX_TRIM,
     board::BATTERY_WATCH}
);

// Ponto de entrada

/// Comece aqui: o ESP-IDF chama depois de subir o FreeRTOS. Dê a partida em
/// tudo e rode o laço principal pra sempre, nesta tarefa
extern "C" void app_main(void) {
    // Dê a partida no hardware: PWM com os motores parados, ADC, LEDs e a
    // memória
    platform::EspPwmOutput::begin_timer(board::PWM_FREQUENCY_HZ);
    _left_in1.begin();
    _left_in2.begin();
    _right_in1.begin();
    _right_in2.begin();
    _battery_input.begin();
    _status_output.begin();
    _alert_output.begin();
    if (!_store.begin()) {
        _logger.warn(
            "main",
            "memória não abriu: ajuste fino não fica guardado"
        );
    }

    // Depois nos componentes e, por último, no barco (que sobe a rede)
    _left_motor.begin();
    _right_motor.begin();
    _status_led.begin();
    _alert_led.begin();
    _boat.begin();

    // Rode o laço principal
    while (true) {
        _boat.update(_clock.millis());
        _clock.sleep_ms(board::LOOP_PERIOD_MS);
    }
}
