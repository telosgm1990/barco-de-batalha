/*
 * O que a página do celular e o barco dizem um pro outro — mensagens de
 * texto curtas, pelo WebSocket.
 *
 * Página -> barco:
 *   "x,y"        joystick, inteiros de -100 a 100, umas 20 vezes por
 *                segundo. Mesmo parado, ela continua mandando "0,0": é o
 *                batimento que o failsafe vigia.
 *   "mode=eco"   troca o modo: eco, fine, full ou war.
 *
 * Barco -> página: {"bat":3.87,"fs":0,"mode":"eco"} — bateria em volts, se o
 * failsafe está armado (1) ou não (0), e o modo atual.
 */
#pragma once

// Includes
#include <cstddef> // size_t

#include "services/drive.hpp" // drive_mode_e

// Enums/structs
namespace services::protocol {

enum class message_e {
    JOYSTICK,
    MODE,
};

struct message_st {
    message_e type;
    float x;           // JOYSTICK: -1 a 1, pro lado
    float y;           // JOYSTICK: -1 a 1, pra frente
    drive_mode_e mode; // MODE
};

struct telemetry_st {
    float battery_volts;
    bool failsafe;
    drive_mode_e mode;
};

// Funções

// Leia uma mensagem da página; false se não for nenhuma das conhecidas
bool parse_message(const char *text, message_st *message);

// Nome curto do modo, o mesmo que a página usa ("eco", "fine"...)
const char *mode_name(drive_mode_e mode);

// Escreva a telemetria em out; devolve quantos caracteres escreveu
int format_telemetry(char *out, size_t size, const telemetry_st &telemetry);

} // namespace services::protocol
