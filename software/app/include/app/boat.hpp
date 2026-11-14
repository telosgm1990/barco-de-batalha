/*
 * O barco: a cada volta do laço, lê o joystick, vigia o failsafe, anda a
 * tração, manda nos motores, pisca o LED e, de tempos em tempos, manda a
 * bateria pra página.
 */
#pragma once

// Includes
#include <cstdint> // uint32_t

#include "components/battery.hpp" // Battery
#include "components/led.hpp"     // Led
#include "components/motor.hpp"   // Motor
#include "services/remote.hpp"    // config_st

// Enums/structs
namespace boat {

// As peças, montadas no bootstrap (main/)
struct parts_st {
    components::Motor *left;
    components::Motor *right;
    components::Battery *battery;
    components::Led *led;
};

// Os números de comportamento — vários ainda a medir na bancada
struct tuning_st {
    uint32_t failsafe_timeout_ms;
    float slew_per_second;
    float deadzone;
    uint32_t telemetry_period_ms;
};

// Funções

// Prepare o barco e suba o controle remoto
void init(
    const parts_st &parts,
    const tuning_st &tuning,
    const remote::config_st &remote_config
);

// Uma volta do laço principal
void update(uint32_t now_ms);

} // namespace boat
