// Includes
#include "services/protocol.hpp" // protocol

#include <cstdio>  // snprintf
#include <cstdlib> // strtol

#include "common/math.hpp" // clamp

// Constantes/variáveis estáticas
static constexpr float JOYSTICK_SCALE = 100.0f;

// Protótipos de funções estáticas
static bool _read_int(const char *text, const char **end, long *value);

// Funções públicas
namespace services::protocol {

bool parse_joystick(const char *text, float *x, float *y) {
    if (text == nullptr || x == nullptr || y == nullptr) {
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

int format_telemetry(
    char *out,
    size_t size,
    float battery_volts,
    bool failsafe
) {
    return snprintf(
        out,
        size,
        "{\"bat\":%.2f,\"fs\":%d}",
        static_cast<double>(battery_volts),
        failsafe ? 1 : 0
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
