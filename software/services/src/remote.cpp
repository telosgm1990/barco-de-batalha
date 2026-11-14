// Includes
#include "services/remote.hpp" // remote

#include <atomic> // std::atomic

#include "platform/clock.hpp"    // millis
#include "platform/http.hpp"     // begin, send_text
#include "platform/log.hpp"      // info
#include "platform/wifi.hpp"     // begin_ap
#include "services/protocol.hpp" // parse_joystick

// Constantes/variáveis estáticas
static constexpr const char *TAG = "remote";

// A página, embutida no firmware pelo CMakeLists.txt (EMBED_TXTFILES)
extern const char _page_start[] asm("_binary_index_html_start");
extern const char _page_end[] asm("_binary_index_html_end");

static remote::config_st _config = {};

// O comando chega pela tarefa do servidor e é lido pelo laço principal
static std::atomic<int32_t> _packed_xy{0};
static std::atomic<uint32_t> _received_ms{0};
static std::atomic<bool> _valid{false};
static std::atomic<int> _client{-1};

// Protótipos de funções estáticas
static void _on_text(const char *text, size_t length, int client);

// Funções públicas
namespace remote {

void init(const config_st &config) {
    _config = config;
    _packed_xy.store(0);
    _received_ms.store(0);
    _valid.store(false);
    _client.store(-1);
}

bool begin() {
    platform::wifi::ap_config_st ap = {
        _config.ssid,
        _config.password,
        _config.channel,
        _config.max_tx_power_qdbm
    };
    if (!platform::wifi::begin_ap(ap)) {
        platform::log::warn(TAG, "Wi-Fi não subiu");
        return false;
    }
    // A página tem um '\0' no fim, que não vai pro navegador
    platform::http::server_config_st server = {
        _page_start,
        static_cast<size_t>(_page_end - _page_start - 1),
        _on_text
    };
    if (!platform::http::begin(server)) {
        platform::log::warn(TAG, "servidor da página não subiu");
        return false;
    }
    platform::log::info(
        TAG,
        "rede \"%s\" no ar: abra http://192.168.4.1",
        _config.ssid
    );
    return true;
}

command_st get_command() {
    int32_t packed      = _packed_xy.load();
    command_st command  = {};
    command.x           = static_cast<int16_t>(packed & 0xFFFF) / 1000.0f;
    command.y           = static_cast<int16_t>(packed >> 16) / 1000.0f;
    command.received_ms = _received_ms.load();
    command.valid       = _valid.load();
    return command;
}

bool is_connected() {
    return platform::http::is_client_open(_client.load());
}

void send_telemetry(const char *text) {
    platform::http::send_text(_client.load(), text);
}

} // namespace remote

// Funções estáticas

// Guarde o comando que chegou, com a hora (em milésimos, pra caber junto)
static void _on_text(const char *text, size_t length, int client) {
    (void)length;
    float x = 0.0f;
    float y = 0.0f;
    if (!protocol::parse_joystick(text, &x, &y)) {
        return;
    }
    int32_t packed =
        (static_cast<int32_t>(static_cast<int16_t>(y * 1000)) << 16) |
        (static_cast<uint16_t>(static_cast<int16_t>(x * 1000)));
    _packed_xy.store(packed);
    _received_ms.store(platform::clock::millis());
    _client.store(client);
    _valid.store(true);
}
