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

#include "app/battery_watch.hpp"          // BatteryWatch
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
static void _test_battery_watch();
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
    _test_battery_watch();
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
    // Curva: sem expo é reta; com expo, o meio fica mais suave
    CHECK(_near(Drive::shape(0.5f, 0.0f), 0.5f));
    CHECK(Drive::shape(0.5f, 1.0f) < 0.2f);
    CHECK(_near(Drive::shape(1.0f, 1.0f), 1.0f));
    CHECK(_near(Drive::shape(-1.0f, 0.6f), -1.0f));
}

static void _test_drive_ramp() {
    // Rampa de 2 por segundo: em 0,1 s anda 0,2
    services::Drive drive(0.05f, {1.0f, 2.0f, 0.0f});
    drive.update(0.1f, 0.0f, 1.0f);
    services::wheels_st w = drive.get_output();
    CHECK(_near(w.left, 0.2f) && _near(w.right, 0.2f));
    // Depois de bastante tempo, chega no alvo e para nele
    for (int i = 0; i < 20; i++) {
        drive.update(0.1f, 0.0f, 1.0f);
    }
    w = drive.get_output();
    CHECK(_near(w.left, 1.0f) && _near(w.right, 1.0f));
    // Perfil com limite de 0,5: a rampa desce até ele
    drive.set_profile({0.5f, 10.0f, 0.0f});
    drive.update(1.0f, 0.0f, 1.0f);
    w = drive.get_output();
    CHECK(_near(w.left, 0.5f) && _near(w.right, 0.5f));
    // Ajuste fino positivo: tira 20% da roda direita
    drive.set_trim(0.2f);
    drive.update(1.0f, 0.0f, 1.0f);
    w = drive.get_output();
    CHECK(_near(w.left, 0.5f) && _near(w.right, 0.4f));
    // Negativo: tira da esquerda, também na ré
    drive.set_trim(-0.2f);
    drive.update(1.0f, 0.0f, -1.0f);
    w = drive.get_output();
    CHECK(_near(w.left, -0.4f) && _near(w.right, -0.5f));
    drive.set_trim(0.0f);
    // stop zera na hora, sem rampa
    drive.stop();
    w = drive.get_output();
    CHECK(_near(w.left, 0.0f) && _near(w.right, 0.0f));
    // Zona morta: joystick quase no meio conta como parado
    services::Drive quick(0.05f, {1.0f, 100.0f, 0.0f});
    quick.update(0.1f, 0.03f, 0.02f);
    w = quick.get_output();
    CHECK(_near(w.left, 0.0f) && _near(w.right, 0.0f));
}

static void _test_protocol() {
    namespace protocol = services::protocol;
    using services::drive_mode_e;

    // Joystick: "x,y" de -100 a 100 vira -1 a 1
    float x = 0.0f;
    float y = 0.0f;
    CHECK(protocol::parse_joystick("0,0", &x, &y));
    CHECK(_near(x, 0.0f) && _near(y, 0.0f));
    CHECK(protocol::parse_joystick("-50,100", &x, &y));
    CHECK(_near(x, -0.5f) && _near(y, 1.0f));
    CHECK(protocol::parse_joystick("300,-300", &x, &y)); // limita em 1
    CHECK(_near(x, 1.0f) && _near(y, -1.0f));
    CHECK(!protocol::parse_joystick("", &x, &y));
    CHECK(!protocol::parse_joystick("10", &x, &y));
    CHECK(!protocol::parse_joystick("10,", &x, &y));
    CHECK(!protocol::parse_joystick("a,b", &x, &y));
    CHECK(!protocol::parse_joystick("10,20,30", &x, &y));
    CHECK(!protocol::parse_joystick(nullptr, &x, &y));

    // PATCH dos ajustes: só o que veio
    protocol::settings_patch_st patch = {};
    CHECK(protocol::parse_settings_patch("{\"mode\":\"war\"}", &patch));
    CHECK(patch.has_mode && patch.mode == drive_mode_e::WAR);
    CHECK(!patch.has_trim);
    CHECK(
        protocol::parse_settings_patch(
            " { \"trim\" : -12 , \"mode\":\"fine\" } ",
            &patch
        )
    );
    CHECK(patch.has_trim && _near(patch.trim, -0.12f));
    CHECK(patch.has_mode && patch.mode == drive_mode_e::PRECISE);
    CHECK(protocol::parse_settings_patch("{\"trim\":500}", &patch));
    CHECK(_near(patch.trim, 1.0f)); // limita em 100%
    // JSON torto, vazio, chave ou valor que não existe: não passa
    CHECK(!protocol::parse_settings_patch("{}", &patch));
    CHECK(!protocol::parse_settings_patch("{\"mode\":\"turbo\"}", &patch));
    CHECK(!protocol::parse_settings_patch("{\"speed\":1}", &patch));
    CHECK(!protocol::parse_settings_patch("{\"trim\":\"x\"}", &patch));
    CHECK(!protocol::parse_settings_patch("{\"trim\":5.5}", &patch));
    CHECK(!protocol::parse_settings_patch("{\"mode\":\"war\"", &patch));
    CHECK(!protocol::parse_settings_patch("{\"mode\":\"war\",}", &patch));
    CHECK(!protocol::parse_settings_patch("{\"mode\":\"war\"} x", &patch));
    CHECK(!protocol::parse_settings_patch("mode=war", &patch));
    CHECK(!protocol::parse_settings_patch(nullptr, &patch));

    // Respostas
    CHECK(strcmp(protocol::mode_name(drive_mode_e::ECONOMY), "eco") == 0);
    char text[80];
    protocol::format_status(
        text,
        sizeof(text),
        {3.874f, true, true, drive_mode_e::FULL, -0.12f}
    );
    CHECK(
        strcmp(
            text,
            "{\"bat\":3.87,\"fs\":1,\"low\":1,\"mode\":\"full\","
            "\"trim\":-12}"
        ) == 0
    );
    protocol::format_settings(text, sizeof(text), drive_mode_e::WAR, 0.05f);
    CHECK(strcmp(text, "{\"mode\":\"war\",\"trim\":5}") == 0);
    protocol::format_error(text, sizeof(text), "não existe");
    CHECK(strcmp(text, "{\"error\":\"não existe\"}") == 0);
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

static void _test_battery_watch() {
    // Avisa abaixo de 3,4 V depois de 2 s seguidos; desliga acima de 3,5 V;
    // abaixo de 1 V é placa no USB, sem bateria
    app::BatteryWatch watch({3.4f, 0.1f, 1.0f, 2000});
    watch.update(0, 3.9f);
    CHECK(!watch.is_low());
    // Afundou numa arrancada e voltou: não avisa
    watch.update(1000, 3.3f);
    watch.update(1500, 3.8f);
    watch.update(3500, 3.3f);
    CHECK(!watch.is_low());
    // Ficou baixa 2 s seguidos: avisa
    watch.update(5000, 3.3f);
    CHECK(!watch.is_low());
    watch.update(5500, 3.35f);
    CHECK(watch.is_low());
    // Subiu um pouco, mas não passou da histerese: continua avisando
    watch.update(6000, 3.45f);
    CHECK(watch.is_low());
    // Passou: desliga
    watch.update(6500, 3.6f);
    CHECK(!watch.is_low());
    // Sem bateria (USB): nunca avisa
    watch.update(7000, 0.0f);
    watch.update(20000, 0.0f);
    CHECK(!watch.is_low());
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
    using platform::http_method_e;
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
    // Antes de qualquer coisa: comando inválido, nada pedido, ninguém
    CHECK(!remote.get_command().valid);
    CHECK(remote.get_settings().mode_seq == 0);
    CHECK(remote.get_settings().trim_seq == 0);
    CHECK(!remote.is_connected());

    // Joystick pelo WebSocket: guarda x, y e a hora do relógio
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

    // GET /api/status antes do barco publicar: failsafe armado, tudo zero
    CHECK(server.request(http_method_e::GET, "/api/status") == 200);
    CHECK(
        server.last_response ==
        "{\"bat\":0.00,\"fs\":1,\"low\":0,\"mode\":\"eco\",\"trim\":0}"
    );
    // Depois de publicar, o retrato novo
    remote.publish_status(
        {3.874f, false, false, services::drive_mode_e::FULL, -0.12f}
    );
    CHECK(server.request(http_method_e::GET, "/api/status") == 200);
    CHECK(
        server.last_response ==
        "{\"bat\":3.87,\"fs\":0,\"low\":0,\"mode\":\"full\",\"trim\":-12}"
    );
    CHECK(server.request(http_method_e::GET, "/api/settings") == 200);
    CHECK(server.last_response == "{\"mode\":\"full\",\"trim\":-12}");

    // PATCH do modo: 204, sobe o contador do modo; não é batimento
    CHECK(
        server.request(
            http_method_e::PATCH,
            "/api/settings",
            "{\"mode\":\"fine\"}"
        ) == 204
    );
    services::settings_st settings = remote.get_settings();
    CHECK(settings.mode == services::drive_mode_e::PRECISE);
    CHECK(settings.mode_seq == 1 && settings.trim_seq == 0);
    CHECK(remote.get_command().received_ms == 1234);
    // PATCH do ajuste fino: o contador dele, sem mexer no do modo
    CHECK(
        server.request(http_method_e::PATCH, "/api/settings", "{\"trim\":7}") ==
        204
    );
    settings = remote.get_settings();
    CHECK(_near(settings.trim, 0.07f));
    CHECK(settings.mode_seq == 1 && settings.trim_seq == 1);
    // PATCH torto: 400 com o erro, e nada muda
    CHECK(
        server.request(
            http_method_e::PATCH,
            "/api/settings",
            "{\"mode\":\"turbo\"}"
        ) == 400
    );
    CHECK(server.last_response == "{\"error\":\"ajuste inválido\"}");
    CHECK(remote.get_settings().mode_seq == 1);
    // Método errado e caminho que não existe
    CHECK(server.request(http_method_e::PATCH, "/api/status", "{}") == 405);
    CHECK(server.request(http_method_e::GET, "/api/nada") == 404);

    // Rede que não sobe: begin avisa no log e devolve false
    fakes::AccessPoint broken;
    broken.works = false;
    services::WifiRemote
        failing(broken, server, clock, logger, page, page + sizeof(page));
    CHECK(!failing.begin());
    CHECK(logger.lines > 0);
}

static void _test_boat() {
    using services::drive_mode_e;
    fakes::Motor left;
    fakes::Motor right;
    fakes::Battery battery;
    fakes::Led led;
    fakes::Remote remote;
    fakes::Logger logger;
    fakes::Store store;
    // Da última vez, o barco puxava pra esquerda: 10% a menos na direita
    store.values["trim"] = 10;
    // Econômico vai até 0,5; os outros até 1 — rampa rápida pra o teste
    app::tuning_st tuning = {
        500,
        0.05f,
        500,
        {{0.5f, 100.0f, 0.0f},
         {0.6f, 100.0f, 0.6f},
         {1.0f, 100.0f, 0.0f},
         {1.0f, 100.0f, 0.2f}},
        drive_mode_e::ECONOMY,
        0.3f,
        {3.4f, 0.1f, 1.0f, 2000}
    };
    app::Boat boat({left, right, battery, led, remote, logger, store}, tuning);

    boat.begin();
    CHECK(remote.started);
    CHECK(boat.get_mode() == drive_mode_e::ECONOMY);
    CHECK(_near(boat.get_trim(), 0.1f));

    // Sem comando nenhum: failsafe armado, motores parados, LED esperando,
    // e ainda não é hora de publicar
    boat.update(100);
    CHECK(_near(left.speed, 0.0f) && _near(right.speed, 0.0f));
    CHECK(led.blink_ms == 1000);
    CHECK(remote.status_count == 0);

    // Comando fresco, celular conectado: anda até o limite do econômico,
    // com o ajuste fino guardado tirando da direita. O failsafe desarmou:
    // publica na hora
    remote.connected = true;
    remote.command   = {0.0f, 1.0f, 150, true};
    boat.update(200);
    CHECK(_near(left.speed, 0.5f) && _near(right.speed, 0.45f));
    CHECK(led.blink_ms == 0);
    CHECK(remote.status_count == 1);
    CHECK(!remote.last_status.failsafe);

    // A página pediu "tudo": o limite sobe, o ajuste fino fica
    remote.settings = {drive_mode_e::FULL, 1, 0.0f, 0};
    remote.command  = {0.0f, 1.0f, 250, true};
    boat.update(300);
    CHECK(boat.get_mode() == drive_mode_e::FULL);
    CHECK(_near(boat.get_trim(), 0.1f));
    CHECK(_near(left.speed, 1.0f) && _near(right.speed, 0.9f));
    CHECK(remote.last_status.mode == drive_mode_e::FULL);

    // Ajuste fino novo, além do máximo: fica no máximo e vai pra memória
    remote.settings = {drive_mode_e::FULL, 1, -0.5f, 1};
    boat.update(320);
    CHECK(_near(boat.get_trim(), -0.3f));
    CHECK(store.values["trim"] == -30);
    CHECK(store.writes == 1);
    CHECK(_near(left.speed, 0.7f) && _near(right.speed, 1.0f));
    CHECK(_near(remote.last_status.trim, -0.3f));
    // O mesmo pedido de novo não grava outra vez nem publica
    int published = remote.status_count;
    boat.update(340);
    CHECK(store.writes == 1);
    CHECK(remote.status_count == published);

    // Sem mudança, publica de período em período (500 ms), com a bateria
    remote.command = {0.0f, 1.0f, 800, true};
    boat.update(810);
    CHECK(remote.status_count == published);
    boat.update(830);
    CHECK(remote.status_count == published + 1);
    CHECK(_near(remote.last_status.battery_volts, 3.9f));
    CHECK(!remote.last_status.low_battery);

    // Comando parou de chegar: passou do tempo, para na hora e publica
    boat.update(1400);
    CHECK(_near(left.speed, 0.0f) && _near(right.speed, 0.0f));
    CHECK(led.blink_ms == 200);
    CHECK(logger.last_line == "failsafe armado: motores parados");
    CHECK(remote.last_status.failsafe);

    // Bateria acabando: depois de 2 s baixa, avisa no log e no retrato
    battery.volts = 3.2f;
    for (uint32_t now = 1900; now <= 3900; now += 500) {
        boat.update(now);
    }
    CHECK(remote.last_status.low_battery);
    CHECK(logger.last_line == "bateria baixa: 3.20 V");
}
