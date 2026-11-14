// Includes
#include "services/protocol.hpp" // protocol

#include <cmath>   // std::lround
#include <cstdio>  // snprintf
#include <cstdlib> // strtol
#include <cstring> // strcmp

#include "common/math.hpp" // clamp

// Constantes/variáveis estáticas
static constexpr float JOYSTICK_SCALE = 100.0f;
static constexpr float TRIM_SCALE     = 100.0f;
static constexpr size_t WORD_MAX      = 16; // chave ou texto no JSON

// Na ordem de drive_mode_e
static constexpr const char *MODE_NAMES[services::DRIVE_MODE_COUNT] = {
    "eco",
    "fine",
    "full",
    "war",
};

// Protótipos de funções estáticas

/**
 * Leia um inteiro com sinal.
 *
 * @param[out] end o primeiro caractere depois do número
 * @return false se não começar com um número
 */
static bool _read_int(const char *text, const char **end, long *value);

/**
 * Leia um texto entre aspas, sem escapes (a API não precisa).
 *
 * @return o caractere depois das aspas de fechamento; nullptr se não for um
 *         texto ou não couber em size
 */
static const char *_read_word(const char *text, char *out, size_t size);

/// Pule espaços, tabs e quebras de linha
static const char *_skip_spaces(const char *text);

/// Ache o modo pelo nome curto; false se não existir
static bool _parse_mode(const char *name, services::drive_mode_e *mode);

/**
 * Leia o valor de uma chave do PATCH e guarde no pedido.
 *
 * @return o caractere depois do valor; nullptr se a chave ou o valor não
 *         servirem
 */
static const char *_read_setting(
    const char *key,
    const char *cursor,
    services::protocol::settings_patch_st *patch
);

// Funções públicas
namespace services::protocol {

bool parse_joystick(const char *text, float *x, float *y) {
    if (text == nullptr) {
        return false;
    }
    long raw_x         = 0;
    long raw_y         = 0;
    const char *cursor = text;
    if (!_read_int(cursor, &cursor, &raw_x) || *cursor != ',') {
        return false;
    }
    if (!_read_int(cursor + 1, &cursor, &raw_y) || *cursor != '\0') {
        return false;
    }
    *x = common::clamp(raw_x / JOYSTICK_SCALE, -1.0f, 1.0f);
    *y = common::clamp(raw_y / JOYSTICK_SCALE, -1.0f, 1.0f);
    return true;
}

bool parse_settings_patch(const char *json, settings_patch_st *patch) {
    if (json == nullptr) {
        return false;
    }
    *patch             = {};
    const char *cursor = _skip_spaces(json);
    if (*cursor != '{') {
        return false;
    }
    cursor = _skip_spaces(cursor + 1);
    if (*cursor == '}') {
        return false; // {} não pede nada
    }
    // Uma volta por chave: "chave": valor, seguido de ',' ou '}'
    while (true) {
        char key[WORD_MAX];
        cursor = _read_word(cursor, key, sizeof(key));
        if (cursor == nullptr) {
            return false;
        }
        cursor = _skip_spaces(cursor);
        if (*cursor != ':') {
            return false;
        }
        cursor = _read_setting(key, _skip_spaces(cursor + 1), patch);
        if (cursor == nullptr) {
            return false;
        }
        cursor = _skip_spaces(cursor);
        if (*cursor == '}') {
            break;
        }
        if (*cursor != ',') {
            return false;
        }
        cursor = _skip_spaces(cursor + 1);
    }
    // Depois do '}', nada além de espaço
    return *_skip_spaces(cursor + 1) == '\0';
}

const char *mode_name(drive_mode_e mode) {
    int index = mode_index(mode);
    if (index < 0 || index >= DRIVE_MODE_COUNT) {
        return "?";
    }
    return MODE_NAMES[index];
}

int format_status(char *out, size_t size, const status_st &status) {
    return snprintf(
        out,
        size,
        "{\"bat\":%.2f,\"fs\":%d,\"mode\":\"%s\",\"trim\":%ld}",
        static_cast<double>(status.battery_volts),
        status.failsafe ? 1 : 0,
        mode_name(status.mode),
        std::lround(status.trim * TRIM_SCALE)
    );
}

int format_settings(char *out, size_t size, drive_mode_e mode, float trim) {
    return snprintf(
        out,
        size,
        "{\"mode\":\"%s\",\"trim\":%ld}",
        mode_name(mode),
        std::lround(trim * TRIM_SCALE)
    );
}

int format_error(char *out, size_t size, const char *message) {
    return snprintf(out, size, "{\"error\":\"%s\"}", message);
}

} // namespace services::protocol

// Funções estáticas

static bool _read_int(const char *text, const char **end, long *value) {
    char *stop = nullptr;
    *value     = strtol(text, &stop, 10);
    if (stop == text) {
        return false;
    }
    *end = stop;
    return true;
}

static const char *_read_word(const char *text, char *out, size_t size) {
    if (*text != '"') {
        return nullptr;
    }
    size_t length = 0;
    for (const char *cursor = text + 1; *cursor != '\0'; cursor++) {
        if (*cursor == '"') {
            out[length] = '\0';
            return cursor + 1;
        }
        if (*cursor == '\\' || length + 1 >= size) {
            return nullptr;
        }
        out[length++] = *cursor;
    }
    return nullptr; // acabou sem fechar as aspas
}

static const char *_skip_spaces(const char *text) {
    while (*text == ' ' || *text == '\t' || *text == '\n' || *text == '\r') {
        text++;
    }
    return text;
}

static bool _parse_mode(const char *name, services::drive_mode_e *mode) {
    for (int i = 0; i < services::DRIVE_MODE_COUNT; i++) {
        if (strcmp(name, MODE_NAMES[i]) == 0) {
            *mode = static_cast<services::drive_mode_e>(i);
            return true;
        }
    }
    return false;
}

static const char *_read_setting(
    const char *key,
    const char *cursor,
    services::protocol::settings_patch_st *patch
) {
    if (strcmp(key, "mode") == 0) {
        char name[WORD_MAX];
        cursor = _read_word(cursor, name, sizeof(name));
        if (cursor == nullptr || !_parse_mode(name, &patch->mode)) {
            return nullptr;
        }
        patch->has_mode = true;
        return cursor;
    }
    if (strcmp(key, "trim") == 0) {
        long raw = 0;
        if (!_read_int(cursor, &cursor, &raw)) {
            return nullptr;
        }
        patch->trim     = common::clamp(raw / TRIM_SCALE, -1.0f, 1.0f);
        patch->has_trim = true;
        return cursor;
    }
    return nullptr; // chave desconhecida
}
