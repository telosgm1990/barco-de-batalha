// Includes
#include "services/protocol.hpp" // protocol

#include <cmath>   // std::lround
#include <cstdio>  // snprintf
#include <cstdlib> // strtol
#include <cstring> // strcmp, strncmp

#include "common/math.hpp" // clamp

// Constantes/variáveis estáticas
static constexpr float JOYSTICK_SCALE    = 100.0f;
static constexpr float TRIM_SCALE        = 100.0f;
static constexpr const char *MODE_PREFIX = "mode=";
static constexpr const char *TRIM_PREFIX = "trim=";

// Na ordem de drive_mode_e
static constexpr const char *MODE_NAMES[services::DRIVE_MODE_COUNT] = {
    "eco",
    "fine",
    "full",
    "war",
};

// Protótipos de funções estáticas
static bool _read_int(const char *text, const char **end, long *value);
static bool _parse_joystick(const char *text, float *x, float *y);
static bool _parse_mode(const char *name, services::drive_mode_e *mode);
static bool _parse_trim(const char *text, float *trim);
static bool _starts_with(const char *text, const char *prefix);

// Funções públicas
namespace services::protocol {

bool parse_message(const char *text, message_st *message) {
    if (text == nullptr || message == nullptr) {
        return false;
    }
    if (_starts_with(text, MODE_PREFIX)) {
        message->type = message_e::MODE;
        return _parse_mode(text + strlen(MODE_PREFIX), &message->mode);
    }
    if (_starts_with(text, TRIM_PREFIX)) {
        message->type = message_e::TRIM;
        return _parse_trim(text + strlen(TRIM_PREFIX), &message->trim);
    }
    message->type = message_e::JOYSTICK;
    return _parse_joystick(text, &message->x, &message->y);
}

const char *mode_name(drive_mode_e mode) {
    int index = mode_index(mode);
    if (index < 0 || index >= DRIVE_MODE_COUNT) {
        return "?";
    }
    return MODE_NAMES[index];
}

int format_telemetry(char *out, size_t size, const telemetry_st &telemetry) {
    return snprintf(
        out,
        size,
        "{\"bat\":%.2f,\"fs\":%d,\"mode\":\"%s\",\"trim\":%ld}",
        static_cast<double>(telemetry.battery_volts),
        telemetry.failsafe ? 1 : 0,
        mode_name(telemetry.mode),
        std::lround(telemetry.trim * TRIM_SCALE)
    );
}

} // namespace services::protocol

// Funções estáticas

// Leia um inteiro com sinal; end aponta pro primeiro caractere depois dele
static bool _read_int(const char *text, const char **end, long *value) {
    char *stop = nullptr;
    *value     = strtol(text, &stop, 10);
    if (stop == text) {
        return false;
    }
    *end = stop;
    return true;
}

// Leia "x,y" e devolva de -1 a 1
static bool _parse_joystick(const char *text, float *x, float *y) {
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

// Ache o modo pelo nome curto
static bool _parse_mode(const char *name, services::drive_mode_e *mode) {
    for (int i = 0; i < services::DRIVE_MODE_COUNT; i++) {
        if (strcmp(name, MODE_NAMES[i]) == 0) {
            *mode = static_cast<services::drive_mode_e>(i);
            return true;
        }
    }
    return false;
}

// Leia o ajuste fino, de -100 a 100, e devolva de -1 a 1
static bool _parse_trim(const char *text, float *trim) {
    long raw           = 0;
    const char *cursor = text;
    if (!_read_int(cursor, &cursor, &raw) || *cursor != '\0') {
        return false;
    }
    *trim = common::clamp(raw / TRIM_SCALE, -1.0f, 1.0f);
    return true;
}

// Diga se o texto começa com o prefixo
static bool _starts_with(const char *text, const char *prefix) {
    return strncmp(text, prefix, strlen(prefix)) == 0;
}
