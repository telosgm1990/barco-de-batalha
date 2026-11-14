/**
 * @file
 * O que o celular e o barco dizem um pro outro.
 *
 * **Joystick**, pelo WebSocket em `/ws`: texto `"x,y"`, inteiros de -100 a
 * 100, umas 20 vezes por segundo. Mesmo parado, a página continua mandando
 * `"0,0"`: é o batimento que o failsafe vigia. WebSocket porque é uma conexão
 * só, aberta o tempo todo — um pedido HTTP por mensagem pesaria demais nesse
 * ritmo.
 *
 * **O resto é uma API REST** em `/api`, com JSON — dá pra testar com o
 * `curl`:
 *
 * - `GET /api/status`: 200 com o retrato do barco,
 *   `{"bat":3.87,"fs":0,"low":0,"mode":"eco","trim":-12}`
 * - `GET /api/settings`: 200 com os ajustes em uso,
 *   `{"mode":"eco","trim":-12}`
 * - `PATCH /api/settings` com `{"mode":"war"}`, `{"trim":-12}` ou os dois:
 *   204; corpo errado dá 400 com `{"error":"..."}`
 *
 * `bat` em volts; `fs` 1 se o failsafe parou os motores; `low` 1 se a
 * bateria está acabando; `mode` é `eco`, `fine`, `full` ou `war`; `trim` é o
 * ajuste fino em por cento (-100 a 100).
 *
 * Conta pura, sem dependência: o WifiRemote usa direto, e os testes também.
 */
#pragma once

// Includes
#include <cstddef> // size_t

#include "services/drive.hpp"  // drive_mode_e
#include "services/remote.hpp" // status_st

// Enums/structs
namespace services::protocol {

/// Pedido de mudança nos ajustes: só muda o que veio
struct settings_patch_st {
    bool has_mode;     ///< veio "mode"
    drive_mode_e mode; ///< o modo pedido
    bool has_trim;     ///< veio "trim"
    float trim;        ///< o ajuste fino pedido, -1 a 1
};

// Funções

/**
 * Leia o joystick, `"x,y"`.
 *
 * @param[out] x de -1 a 1 (o que passar de 100 fica em 1)
 * @param[out] y de -1 a 1
 * @return false se não for um joystick
 */
bool parse_joystick(const char *text, float *x, float *y);

/**
 * Leia o corpo de um `PATCH /api/settings`.
 *
 * Aceita só as chaves `mode` (texto) e `trim` (inteiro); qualquer outra coisa
 * é erro, pra um erro de digitação não passar calado.
 *
 * @return false se o JSON for inválido, vazio ou tiver chave desconhecida
 */
bool parse_settings_patch(const char *json, settings_patch_st *patch);

/// Nome curto do modo, o mesmo da API (`"eco"`, `"fine"`...)
const char *mode_name(drive_mode_e mode);

// Respostas da API: escrevem o JSON em out e devolvem quantos caracteres
// escreveram (como o snprintf)

/// Resposta do GET /api/status
int format_status(char *out, size_t size, const status_st &status);

/// Resposta do GET /api/settings
int format_settings(char *out, size_t size, drive_mode_e mode, float trim);

/// Corpo de um erro: {"error":"message"}
int format_error(char *out, size_t size, const char *message);

} // namespace services::protocol
