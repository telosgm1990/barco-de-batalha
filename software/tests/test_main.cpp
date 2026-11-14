/*
 * Testes no PC da tração, do protocolo e do failsafe.
 *
 * Sem biblioteca de teste: CHECK conta as falhas e o programa sai com erro se
 * alguma falhar.
 */

// Includes
#include <cmath>   // std::fabs
#include <cstdint> // uint32_t
#include <cstdio>  // printf
#include <cstring> // strcmp

#include "app/failsafe.hpp"      // Failsafe
#include "common/math.hpp"       // approach, clamp, deadzone
#include "services/drive.hpp"    // drive
#include "services/protocol.hpp" // protocol

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

// Ponto de entrada
int main() {
    _test_math();
    _test_mix();
    _test_drive_ramp();
    _test_protocol();
    _test_failsafe();
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
    // Frente: os dois motores pra frente
    drive::wheels_st w = drive::mix(0.0f, 1.0f);
    CHECK(_near(w.left, 1.0f) && _near(w.right, 1.0f));
    // Pro lado: gira no lugar, como tanque
    w = drive::mix(1.0f, 0.0f);
    CHECK(_near(w.left, 1.0f) && _near(w.right, -1.0f));
    // Diagonal: nenhuma roda passa de 1, e a proporção fica
    w = drive::mix(1.0f, 1.0f);
    CHECK(_near(w.left, 1.0f) && _near(w.right, 0.0f));
    w = drive::mix(0.5f, 1.0f);
    CHECK(_near(w.left, 1.0f) && _near(w.right, 0.5f / 1.5f));
    // Ré
    w = drive::mix(0.0f, -1.0f);
    CHECK(_near(w.left, -1.0f) && _near(w.right, -1.0f));
}

static void _test_drive_ramp() {
    // Rampa de 2 por segundo: em 0,1 s anda 0,2
    drive::init(2.0f, 0.05f);
    drive::update(0.1f, 0.0f, 1.0f);
    drive::wheels_st w = drive::get_output();
    CHECK(_near(w.left, 0.2f) && _near(w.right, 0.2f));
    // Depois de bastante tempo, chega no alvo e para nele
    for (int i = 0; i < 20; i++) {
        drive::update(0.1f, 0.0f, 1.0f);
    }
    w = drive::get_output();
    CHECK(_near(w.left, 1.0f) && _near(w.right, 1.0f));
    // Zona morta: joystick quase no meio conta como parado
    drive::init(100.0f, 0.05f);
    drive::update(0.1f, 0.03f, 0.02f);
    w = drive::get_output();
    CHECK(_near(w.left, 0.0f) && _near(w.right, 0.0f));
    // stop zera na hora, sem rampa
    drive::init(2.0f, 0.0f);
    drive::update(1.0f, 0.0f, 1.0f);
    drive::stop();
    w = drive::get_output();
    CHECK(_near(w.left, 0.0f) && _near(w.right, 0.0f));
}

static void _test_protocol() {
    float x = 9.0f;
    float y = 9.0f;
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
