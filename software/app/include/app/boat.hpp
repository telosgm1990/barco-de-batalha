/*
 * O barco: a cada volta do laço, lê o joystick, vigia o failsafe, anda a
 * tração, manda nos motores, pisca o LED e, de tempos em tempos, manda a
 * bateria pra página. Os ajustes que a página pede (modo e ajuste fino)
 * entram na tração; o ajuste fino fica guardado na memória da placa.
 *
 * Recebe tudo pronto, por referência, como as abstrações das camadas de
 * baixo: os motores, a bateria e o LED de components/, o controle remoto de
 * services/, o log e a memória da platform/. Na placa, quem monta é o main/;
 * nos testes do PC, peças falsas.
 */
#pragma once

// Includes
#include <cstdint> // uint32_t

#include "app/failsafe.hpp"             // Failsafe
#include "components/battery.hpp"       // Battery
#include "components/led.hpp"           // Led
#include "components/motor.hpp"         // Motor
#include "platform/key_value_store.hpp" // KeyValueStore
#include "platform/logger.hpp"          // Logger
#include "services/drive.hpp"           // Drive
#include "services/remote.hpp"          // Remote

// Enums/structs
namespace app {

// Os números de comportamento — vários ainda a medir na bancada
struct tuning_st {
    uint32_t failsafe_timeout_ms;
    float deadzone;
    uint32_t telemetry_period_ms;
    // Um perfil por modo, na ordem de services::drive_mode_e
    services::drive_profile_st profiles[services::DRIVE_MODE_COUNT];
    services::drive_mode_e initial_mode;
    float max_trim; // ajuste fino máximo, de 0 a 1, pros dois lados
};

// As peças que o barco usa, já montadas
struct parts_st {
    components::Motor &left;
    components::Motor &right;
    components::Battery &battery;
    components::Led &led;
    services::Remote &remote;
    platform::Logger &logger;
    platform::KeyValueStore &store;
};

// Classes

class Boat {
  public:
    // Receba as peças e os números
    constexpr Boat(const parts_st &parts, const tuning_st &tuning)
        : _parts(parts), _tuning(tuning), _failsafe(tuning.failsafe_timeout_ms),
          _drive(
              tuning.deadzone,
              tuning.profiles[services::mode_index(tuning.initial_mode)]
          ),
          _mode(tuning.initial_mode) {
    }

    // Pegue o ajuste fino guardado e suba o controle remoto (rede e página)
    void begin();

    // Uma volta do laço principal
    void update(uint32_t now_ms);

    // Modo em que o barco está andando
    services::drive_mode_e get_mode() const;

    // Ajuste fino em uso, de -1 a 1
    float get_trim() const;

  private:
    void _apply_settings();
    void _set_trim(float trim);
    void _update_led(uint32_t now_ms, bool connected, bool tripped);
    void _send_telemetry(bool tripped);

    parts_st _parts;
    tuning_st _tuning;
    Failsafe _failsafe;
    services::Drive _drive;
    services::drive_mode_e _mode;
    float _trim                 = 0.0f;
    uint32_t _mode_seq          = 0;
    uint32_t _trim_seq          = 0;
    uint32_t _last_update_ms    = 0;
    uint32_t _last_telemetry_ms = 0;
    bool _was_tripped           = true;
};

} // namespace app
