// Includes
#include "services/protocol.hpp" // protocol

#include <cstdio>  // snprintf
#include <cstdlib> // strtol
#include <cstring> // strcmp, strncmp

#include "common/math.hpp" // clamp

// Constantes/variáveis estáticas
static constexpr float JOYSTICK_SCALE    = 100.0f;
static constexpr const char *MODE_PREFIX = "mode=";

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

// Funções públicas
namespace services::protocol {

bool parse_message(const char *text, message_st *message) {
    if (text == nullptr || message == nullptr) {
        return false;
    }
    size_t prefix_length = strlen(MODE_PREFIX);
    if (strncmp(text, MODE_PREFIX, prefix_length) == 0) {
        message->type = message_e::MODE;
        return _parse_mode(text + prefix_length, &message->mode);
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
        "{\"bat\":%.2f,\"fs\":%d,\"mode\":\"%s\"}",
        static_cast<double>(telemetry.battery_volts),
        telemetry.failsafe ? 1 : 0,
        mode_name(telemetry.mode)
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
