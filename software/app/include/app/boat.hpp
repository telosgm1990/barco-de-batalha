/*
 * O barco: a cada volta do laço, lê o joystick, vigia o failsafe, anda a
 * tração, manda nos motores, pisca o LED e, de tempos em tempos, manda a
 * bateria pra página.
 *
 * Recebe tudo pronto, por referência, como as abstrações das camadas de
 * baixo: os motores, a bateria e o LED de components/, o controle remoto de
 * services/ e o log da platform/. Na placa, quem monta é o main/; nos testes
 * do PC, peças falsas.
 */
#pragma once

// Includes
#include <cstdint> // uint32_t

#include "app/failsafe.hpp"       // Failsafe
#include "components/battery.hpp" // Battery
#include "components/led.hpp"     // Led
#include "components/motor.hpp"   // Motor
#include "platform/logger.hpp"    // Logger
#include "services/drive.hpp"     // Drive
#include "services/remote.hpp"    // Remote

// Enums/structs
namespace app {

// Os números de comportamento — vários ainda a medir na bancada
struct tuning_st {
    uint32_t failsafe_timeout_ms;
    float slew_per_second;
    float deadzone;
    uint32_t telemetry_period_ms;
};

// As peças que o barco usa, já montadas
struct parts_st {
    components::Motor &left;
    components::Motor &right;
    components::Battery &battery;
    components::Led &led;
    services::Remote &remote;
    platform::Logger &logger;
};

// Classes

class Boat {
  public:
    // Receba as peças e os números
    constexpr Boat(const parts_st &parts, const tuning_st &tuning)
        : _parts(parts), _tuning(tuning), _failsafe(tuning.failsafe_timeout_ms),
          _drive(tuning.slew_per_second, tuning.deadzone) {
    }

    // Suba o controle remoto (rede e página)
    void begin();

    // Uma volta do laço principal
    void update(uint32_t now_ms);

  private:
    void _update_led(uint32_t now_ms, bool connected, bool tripped);
    void _send_telemetry(bool tripped);

    parts_st _parts;
    tuning_st _tuning;
    Failsafe _failsafe;
    services::Drive _drive;
    uint32_t _last_update_ms    = 0;
    uint32_t _last_telemetry_ms = 0;
    bool _was_tripped           = true;
};

} // namespace app
