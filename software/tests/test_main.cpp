/*
 * Testes no PC, sem placa: a conta pura, os componentes, o controle remoto e
 * o barco inteiro — cada um com peças falsas (fakes.hpp) no lugar das
 * abstrações da camada de baixo.
 *
 * Sem biblioteca de teste: CHECK conta as falhas e o programa sai com erro se
 * alguma falhar.
 */

// Includes
#include <cmath>   // std::fabs
#include <cstdint> // uint32_t
#include <cstdio>  // printf
#include <cstring> // strcmp, strlen
#include <string>  // std::string

#include "app/boat.hpp"                   // Boat
#include "app/failsafe.hpp"               // Failsafe
#include "common/math.hpp"                // approach, clamp, deadzone
#include "components/digital_led.hpp"     // DigitalLed
#include "components/divider_battery.hpp" // DividerBattery
#include "components/h_bridge_motor.hpp"  // HBridgeMotor
#include "fakes.hpp"                      // peças falsas
#include "services/drive.hpp"             // Drive
#include "services/protocol.hpp"          // protocol
#include "services/wifi_remote.hpp"       // WifiRemote

// Defines
#define CHECK(condition)                                                  \
    do {                                                                  \
        _checks++;                                                        \
        if (!(condition)) {                                               \
            _failures++;                                                  \
            printf("FALHOU %s:%d  %s\n", __FILE__, __LINE__, #condition); \
        }                                                                 \
    } while (0)

// Constantes/variáveis estáticas
static int _checks   = 0;
static int _failures = 0;

// Protótipos de funções estáticas
static bool _near(float a, float b);
static void _test_math();
static void _test_mix();
static void _test_drive_ramp();
static void _test_protocol();
static void _test_failsafe();
static void _test_motor();
static void _test_led();
static void _test_battery();
static void _test_wifi_remote();
static void _test_boat();

// Ponto de entrada
int main() {
    _test_math();
    _test_mix();
    _test_drive_ramp();
    _test_protocol();
    _test_failsafe();
    _test_motor();
    _test_led();
    _test_battery();
    _test_wifi_remote();
    _test_boat();
    printf("%d checagens, %d falhas\n", _checks, _failures);
    return _failures == 0 ? 0 : 1;
}

// Funções estáticas
static bool _near(float a, float b) {
    return std::fabs(a - b) < 1e-4f;
}

static void _test_math() {
    CHECK(_near(common::clamp(2.0f, -1.0f, 1.0f), 1.0f));
    CHECK(_near(common::clamp(-2.0f, -1.0f, 1.0f), -1.0f));
    CHECK(_near(common::approach(0.0f, 1.0f, 0.25f), 0.25f));
    CHECK(_near(common::approach(0.9f, 1.0f, 0.25f), 1.0f));
    CHECK(_near(common::approach(0.0f, -1.0f, 0.25f), -0.25f));
    CHECK(_near(common::deadzone(0.03f, 0.05f), 0.0f));
    CHECK(_near(common::deadzone(-0.3f, 0.05f), -0.3f));
}

static void _test_mix() {
    using services::Drive;
    // Frente: os dois motores pra frente
    services::wheels_st w = Drive::mix(0.0f, 1.0f);
    CHECK(_near(w.left, 1.0f) && _near(w.right, 1.0f));
    // Pro lado: gira no lugar, como tanque
    w = Drive::mix(1.0f, 0.0f);
    CHECK(_near(w.left, 1.0f) && _near(w.right, -1.0f));
    // Diagonal: nenhuma roda passa de 1, e a proporção fica
    w = Drive::mix(1.0f, 1.0f);
    CHECK(_near(w.left, 1.0f) && _near(w.right, 0.0f));
    w = Drive::mix(0.5f, 1.0f);
    CHECK(_near(w.left, 1.0f) && _near(w.right, 0.5f / 1.5f));
    // Ré
    w = Drive::mix(0.0f, -1.0f);
    CHECK(_near(w.left, -1.0f) && _near(w.right, -1.0f));
}

static void _test_drive_ramp() {
    // Rampa de 2 por segundo: em 0,1 s anda 0,2
    services::Drive drive(2.0f, 0.05f);
    drive.update(0.1f, 0.0f, 1.0f);
    services::wheels_st w = drive.get_output();
    CHECK(_near(w.left, 0.2f) && _near(w.right, 0.2f));
    // Depois de bastante tempo, chega no alvo e para nele
    for (int i = 0; i < 20; i++) {
        drive.update(0.1f, 0.0f, 1.0f);
    }
    w = drive.get_output();
    CHECK(_near(w.left, 1.0f) && _near(w.right, 1.0f));
    // stop zera na hora, sem rampa
    drive.stop();
    w = drive.get_output();
    CHECK(_near(w.left, 0.0f) && _near(w.right, 0.0f));
    // Zona morta: joystick quase no meio conta como parado
    services::Drive quick(100.0f, 0.05f);
    quick.update(0.1f, 0.03f, 0.02f);
    w = quick.get_output();
    CHECK(_near(w.left, 0.0f) && _near(w.right, 0.0f));
}

static void _test_protocol() {
    namespace protocol = services::protocol;
    float x            = 9.0f;
    float y            = 9.0f;
    CHECK(protocol::parse_joystick("0,0", &x, &y));
    CHECK(_near(x, 0.0f) && _near(y, 0.0f));
    CHECK(protocol::parse_joystick("-50,100", &x, &y));
    CHECK(_near(x, -0.5f) && _near(y, 1.0f));
    // Fora da faixa: limita em 1
    CHECK(protocol::parse_joystick("300,-300", &x, &y));
    CHECK(_near(x, 1.0f) && _near(y, -1.0f));
    // Lixo não passa
    CHECK(!protocol::parse_joystick("", &x, &y));
    CHECK(!protocol::parse_joystick("10", &x, &y));
    CHECK(!protocol::parse_joystick("10,", &x, &y));
    CHECK(!protocol::parse_joystick("a,b", &x, &y));
    CHECK(!protocol::parse_joystick("10,20,30", &x, &y));
    CHECK(!protocol::parse_joystick(nullptr, &x, &y));

    char text[48];
    protocol::format_telemetry(text, sizeof(text), 3.874f, true);
    CHECK(strcmp(text, "{\"bat\":3.87,\"fs\":1}") == 0);
    protocol::format_telemetry(text, sizeof(text), 4.2f, false);
    CHECK(strcmp(text, "{\"bat\":4.20,\"fs\":0}") == 0);
}

static void _test_failsafe() {
    app::Failsafe failsafe(500);
    // Ligou e ainda não chegou nada: armado
    CHECK(failsafe.is_tripped());
    failsafe.update(1000, 0, false);
    CHECK(failsafe.is_tripped());
    // Comando fresco: desarmado
    failsafe.update(1000, 900, true);
    CHECK(!failsafe.is_tripped());
    // No limite ainda vale; passou dele, arma
    failsafe.update(1400, 900, true);
    CHECK(!failsafe.is_tripped());
    failsafe.update(1401, 900, true);
    CHECK(failsafe.is_tripped());
    // Relógio de 32 bits dando a volta não engana o failsafe
    uint32_t last = 0xFFFFFF00u;
    failsafe.update(last + 100, last, true);
    CHECK(!failsafe.is_tripped());
    failsafe.update(last + 1000, last, true);
    CHECK(failsafe.is_tripped());
}

static void _test_motor() {
    fakes::Pwm in1;
    fakes::Pwm in2;
    components::HBridgeMotor motor(in1, in2, false);
    // Começa parado
    motor.begin();
    CHECK(_near(in1.last_duty, 0.0f) && _near(in2.last_duty, 0.0f));
    // Pra frente: PWM na entrada 1, a 2 em zero
    motor.set_speed(0.6f);
    CHECK(_near(in1.last_duty, 0.6f) && _near(in2.last_duty, 0.0f));
    // Pra trás: o contrário
    motor.set_speed(-0.4f);
    CHECK(_near(in1.last_duty, 0.0f) && _near(in2.last_duty, 0.4f));
    // Passou de 1: limita
    motor.set_speed(3.0f);
    CHECK(_near(in1.last_duty, 1.0f));
    // Invertido: frente vira ré
    fakes::Pwm a;
    fakes::Pwm b;
    components::HBridgeMotor reversed(a, b, true);
    reversed.set_speed(0.5f);
    CHECK(_near(a.last_duty, 0.0f) && _near(b.last_duty, 0.5f));
}

static void _test_led() {
    fakes::Digital pin;
    components::DigitalLed led(pin, false);
    led.begin();
    CHECK(!pin.level);
    // Aceso fixo
    led.set_blink(0);
    led.update(123);
    CHECK(pin.level);
    // Piscando: aceso na primeira metade do período, apagado na segunda
    led.set_blink(1000);
    led.update(100);
    CHECK(pin.level);
    led.update(600);
    CHECK(!pin.level);
    led.set_off();
    led.update(100);
    CHECK(!pin.level);
    // Aceso em nível baixo: tudo invertido no pino
    fakes::Digital low_pin;
    components::DigitalLed low_led(low_pin, true);
    low_led.begin();
    CHECK(low_pin.level);
    low_led.set_blink(0);
    low_led.update(0);
    CHECK(!low_pin.level);
}

static void _test_battery() {
    fakes::Analog input;
    components::DividerBattery battery(input, 2.0f);
    // 1950 mV no pino, divisor por 2: 3,9 V na bateria, com média de amostras
    input.millivolts = 1950;
    CHECK(_near(battery.read_volts(), 3.9f));
    CHECK(input.reads > 1);
    // Leitura que falha devolve negativo
    input.millivolts = -1;
    CHECK(battery.read_volts() < 0.0f);
}

static void _test_wifi_remote() {
    fakes::AccessPoint access_point;
    fakes::WebServer server;
    fakes::Clock clock;
    fakes::Logger logger;
    // A página de verdade vem embutida; aqui, uma qualquer, com o '\0'
    static const char page[] = "<html>joystick</html>";
    services::WifiRemote
        remote(access_point, server, clock, logger, page, page + sizeof(page));

    // Sobe a rede e entrega a página sem o '\0'
    CHECK(remote.begin());
    CHECK(access_point.started);
    CHECK(server.served_page == "<html>joystick</html>");
    // Antes de qualquer mensagem: comando inválido, ninguém conectado
    CHECK(!remote.get_command().valid);
    CHECK(!remote.is_connected());

    // Chegou um comando: guarda x, y e a hora do relógio
    clock.now_ms       = 1234;
    server.open_client = 7;
    server.receive("50,-100", 7);
    services::command_st command = remote.get_command();
    CHECK(command.valid);
    CHECK(_near(command.x, 0.5f) && _near(command.y, -1.0f));
    CHECK(command.received_ms == 1234);
    CHECK(remote.is_connected());

    // Mensagem estragada não mexe no comando
    clock.now_ms = 2000;
    server.receive("lixo", 7);
    CHECK(remote.get_command().received_ms == 1234);

    // Telemetria vai pro cliente que mandou o comando
    remote.send_telemetry("{\"bat\":3.90,\"fs\":0}");
    CHECK(server.last_client == 7);
    CHECK(server.last_text == "{\"bat\":3.90,\"fs\":0}");

    // Rede que não sobe: begin avisa no log e devolve false
    fakes::AccessPoint broken;
    broken.works = false;
    services::WifiRemote
        failing(broken, server, clock, logger, page, page + sizeof(page));
    CHECK(!failing.begin());
    CHECK(logger.lines > 0);
}

static void _test_boat() {
    fakes::Motor left;
    fakes::Motor right;
    fakes::Battery battery;
    fakes::Led led;
    fakes::Remote remote;
    fakes::Logger logger;
    app::Boat boat(
        {left, right, battery, led, remote, logger},
        {500, 100.0f, 0.05f, 500}
    );

    boat.begin();
    CHECK(remote.started);

    // Sem comando nenhum: failsafe armado, motores parados, LED esperando
    boat.update(100);
    CHECK(_near(left.speed, 0.0f) && _near(right.speed, 0.0f));
    CHECK(led.blink_ms == 1000);

    // Comando fresco, celular conectado: anda, LED aceso
    remote.connected = true;
    remote.command   = {0.0f, 1.0f, 150, true};
    boat.update(200);
    CHECK(left.speed > 0.5f && right.speed > 0.5f);
    CHECK(led.blink_ms == 0);
    // Telemetria só depois de um período inteiro (500 ms), com a bateria
    CHECK(remote.telemetry_count == 0);
    remote.command = {0.0f, 1.0f, 550, true};
    boat.update(600);
    CHECK(remote.telemetry_count == 1);
    CHECK(remote.last_telemetry == "{\"bat\":3.90,\"fs\":0}");

    // Comando parou de chegar: passou do tempo, para na hora
    boat.update(1200);
    CHECK(_near(left.speed, 0.0f) && _near(right.speed, 0.0f));
    CHECK(led.blink_ms == 200);
    CHECK(logger.last_line == "failsafe armado: motores parados");
}
