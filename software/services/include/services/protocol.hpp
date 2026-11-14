/*
 * O que a página do celular e o barco dizem um pro outro.
 *
 * Página -> barco: "x,y", inteiros de -100 a 100, umas 20 vezes por segundo.
 * Mesmo parado, ela continua mandando "0,0": é o batimento que o failsafe
 * vigia.
 *
 * Barco -> página: {"bat":3.87,"fs":0} — bateria em volts e se o failsafe
 * está desarmado (0) ou armado (1).
 */
#pragma once

// Includes
#include <cstddef> // size_t

// Funções
namespace protocol {

// Leia "x,y" e devolva de -1 a 1; false se a mensagem não for isso
bool parse_joystick(const char *text, float *x, float *y);

// Escreva a telemetria em out; devolve quantos caracteres escreveu
int format_telemetry(
    char *out,
    size_t size,
    float battery_volts,
    bool failsafe
);

} // namespace protocol
