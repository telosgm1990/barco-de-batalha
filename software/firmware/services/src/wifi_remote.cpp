// Includes
#include "services/wifi_remote.hpp" // WifiRemote

#include <cmath>   // std::lround
#include <cstring> // strcmp

#include "services/protocol.hpp" // parse_*, format_*

// Constantes/variáveis estáticas
static constexpr const char *TAG = "remote";

// Joystick e ajuste fino viajam em inteiros: milésimos e por cento
static constexpr float XY_SCALE   = 1000.0f;
static constexpr float TRIM_SCALE = 100.0f;

// Protótipos de funções estáticas

/// Junte x e y (de -1 a 1) num inteiro só: y em cima, x embaixo
static int32_t _pack_xy(float x, float y);

/// Separe o que _pack_xy juntou
static void _unpack_xy(int32_t packed, float *x, float *y);

// Funções públicas
namespace services {

bool WifiRemote::begin() {
    if (!_access_point.begin()) {
        _logger.warn(TAG, "Wi-Fi não subiu");
        return false;
    }
    // A página tem um '\0' no fim, que não vai pro navegador
    size_t page_length = static_cast<size_t>(_page_end - _page_start);
    if (page_length > 0 && _page_start[page_length - 1] == '\0') {
        page_length--;
    }
    if (!_server.begin(_page_start, page_length, *this, *this)) {
        _logger.warn(TAG, "servidor da página não subiu");
        return false;
    }
    _logger.info(TAG, "rede no ar: abra http://192.168.4.1");
    return true;
}

command_st WifiRemote::get_command() {
    command_st command = {};
    _unpack_xy(_packed_xy.load(), &command.x, &command.y);
    command.received_ms = _received_ms.load();
    command.valid       = _valid.load();
    return command;
}

settings_st WifiRemote::get_settings() {
    settings_st settings = {};
    settings.mode        = static_cast<drive_mode_e>(_mode.load());
    settings.mode_seq    = _mode_seq.load();
    settings.trim        = _trim_percent.load() / TRIM_SCALE;
    settings.trim_seq    = _trim_seq.load();
    return settings;
}

bool WifiRemote::is_connected() {
    return _server.is_client_open(_client.load());
}

void WifiRemote::publish_status(const status_st &status) {
    _status_mv.store(
        static_cast<int32_t>(std::lround(status.battery_volts * 1000))
    );
    _status_failsafe.store(status.failsafe);
    _status_low.store(status.low_battery);
    _status_mode.store(mode_index(status.mode));
    _status_trim_percent.store(
        static_cast<int>(std::lround(status.trim * TRIM_SCALE))
    );
}

// Guarde o joystick com a hora: é o batimento que o failsafe vigia
void WifiRemote::on_text(const char *text, size_t length, int client) {
    (void)length;
    float x = 0.0f;
    float y = 0.0f;
    if (!protocol::parse_joystick(text, &x, &y)) {
        return;
    }
    _packed_xy.store(_pack_xy(x, y));
    _received_ms.store(_clock.millis());
    _valid.store(true);
    _client.store(client);
}

// A API: um recurso por caminho, um método por ação
int WifiRemote::on_request(
    platform::http_method_e method,
    const char *path,
    const char *body,
    char *response,
    size_t size
) {
    using platform::http_method_e;
    if (strcmp(path, "/api/status") == 0) {
        if (method != http_method_e::GET) {
            protocol::format_error(response, size, "use GET");
            return platform::HTTP_METHOD_NOT_ALLOWED;
        }
        protocol::format_status(response, size, _load_status());
        return platform::HTTP_OK;
    }
    if (strcmp(path, "/api/settings") == 0) {
        if (method == http_method_e::PATCH) {
            return _patch_settings(body, response, size);
        }
        status_st status = _load_status();
        protocol::format_settings(response, size, status.mode, status.trim);
        return platform::HTTP_OK;
    }
    protocol::format_error(response, size, "não existe");
    return platform::HTTP_NOT_FOUND;
}

// Guarde o que veio e suba o contador de cada ajuste; quem aplica é o barco,
// na próxima volta do laço — por isso a resposta não tem corpo
int WifiRemote::_patch_settings(const char *body, char *response, size_t size) {
    protocol::settings_patch_st patch = {};
    if (!protocol::parse_settings_patch(body, &patch)) {
        protocol::format_error(response, size, "ajuste inválido");
        return platform::HTTP_BAD_REQUEST;
    }
    if (patch.has_mode) {
        _mode.store(mode_index(patch.mode));
        _mode_seq.fetch_add(1);
    }
    if (patch.has_trim) {
        _trim_percent.store(
            static_cast<int>(std::lround(patch.trim * TRIM_SCALE))
        );
        _trim_seq.fetch_add(1);
    }
    response[0] = '\0';
    return platform::HTTP_NO_CONTENT;
}

status_st WifiRemote::_load_status() const {
    status_st status     = {};
    status.battery_volts = _status_mv.load() / 1000.0f;
    status.failsafe      = _status_failsafe.load();
    status.low_battery   = _status_low.load();
    status.mode          = static_cast<drive_mode_e>(_status_mode.load());
    status.trim          = _status_trim_percent.load() / TRIM_SCALE;
    return status;
}

} // namespace services

// Funções estáticas

static int32_t _pack_xy(float x, float y) {
    auto to_int16 = [](float value) {
        return static_cast<int16_t>(std::lround(value * XY_SCALE));
    };
    return static_cast<int32_t>(
        (static_cast<uint32_t>(static_cast<uint16_t>(to_int16(y))) << 16) |
        static_cast<uint16_t>(to_int16(x))
    );
}

static void _unpack_xy(int32_t packed, float *x, float *y) {
    *x = static_cast<int16_t>(packed & 0xFFFF) / XY_SCALE;
    *y = static_cast<int16_t>((packed >> 16) & 0xFFFF) / XY_SCALE;
}
