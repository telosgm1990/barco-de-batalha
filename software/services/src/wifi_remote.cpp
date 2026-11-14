// Includes
#include "services/wifi_remote.hpp" // WifiRemote

#include "services/protocol.hpp" // parse_message

// Constantes/variáveis estáticas
static constexpr const char *TAG = "remote";

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
    if (!_server.begin(_page_start, page_length, *this)) {
        _logger.warn(TAG, "servidor da página não subiu");
        return false;
    }
    _logger.info(TAG, "rede no ar: abra http://192.168.4.1");
    return true;
}

command_st WifiRemote::get_command() {
    int32_t packed       = _packed_xy.load();
    command_st command   = {};
    command.x            = static_cast<int16_t>(packed & 0xFFFF) / 1000.0f;
    command.y            = static_cast<int16_t>(packed >> 16) / 1000.0f;
    command.received_ms  = _received_ms.load();
    command.valid        = _valid.load();
    command.mode         = static_cast<drive_mode_e>(_mode.load());
    command.settings_seq = _settings_seq.load();
    return command;
}

bool WifiRemote::is_connected() {
    return _server.is_client_open(_client.load());
}

void WifiRemote::send_telemetry(const char *text) {
    _server.send_text(_client.load(), text);
}

// Guarde o que chegou: joystick (com a hora, em milésimos pra caber junto) ou
// ajuste — só o joystick conta como batimento pro failsafe
void WifiRemote::on_text(const char *text, size_t length, int client) {
    (void)length;
    protocol::message_st message = {};
    if (!protocol::parse_message(text, &message)) {
        return;
    }
    _client.store(client);
    switch (message.type) {
    case protocol::message_e::JOYSTICK: {
        int32_t packed =
            (static_cast<int32_t>(static_cast<int16_t>(message.y * 1000))
             << 16) |
            static_cast<uint16_t>(static_cast<int16_t>(message.x * 1000));
        _packed_xy.store(packed);
        _received_ms.store(_clock.millis());
        _valid.store(true);
        break;
    }
    case protocol::message_e::MODE:
        _mode.store(mode_index(message.mode));
        _settings_seq.fetch_add(1);
        break;
    }
}

} // namespace services
