/*
 * Controle remoto pelo celular — implementa Remote.
 *
 * O barco cria a rede, serve a página do joystick e recebe os comandos por
 * WebSocket. Tudo o que ele usa da placa chega pronto, por referência, como
 * as abstrações da platform/: a rede, o servidor, o relógio e o log.
 */
#pragma once

// Includes
#include <atomic>  // std::atomic
#include <cstddef> // size_t
#include <cstdint> // int32_t, uint32_t

#include "platform/access_point.hpp" // AccessPoint
#include "platform/clock.hpp"        // Clock
#include "platform/logger.hpp"       // Logger
#include "platform/web_server.hpp"   // WebServer, TextListener
#include "services/remote.hpp"       // Remote

// Classes
namespace services {

class WifiRemote final : public Remote, private platform::TextListener {
  public:
    // Receba as peças da placa e a página (do início ao fim, com o '\0')
    constexpr WifiRemote(
        platform::AccessPoint &access_point,
        platform::WebServer &server,
        platform::Clock &clock,
        platform::Logger &logger,
        const char *page_start,
        const char *page_end
    )
        : _access_point(access_point), _server(server), _clock(clock),
          _logger(logger), _page_start(page_start), _page_end(page_end) {
    }

    // Suba a rede e o servidor da página
    bool begin() override;

    // Último comando que chegou do joystick
    command_st get_command() override;

    // Diga se tem um celular com a página aberta
    bool is_connected() override;

    // Mande a telemetria pra página, se tiver alguém ouvindo
    void send_telemetry(const char *text) override;

  private:
    // Chegou mensagem da página: guarde o comando
    void on_text(const char *text, size_t length, int client) override;

    platform::AccessPoint &_access_point;
    platform::WebServer &_server;
    platform::Clock &_clock;
    platform::Logger &_logger;
    const char *_page_start;
    const char *_page_end;

    // O comando chega pela tarefa do servidor e é lido pelo laço principal
    std::atomic<int32_t> _packed_xy{0};
    std::atomic<uint32_t> _received_ms{0};
    std::atomic<bool> _valid{false};
    std::atomic<int> _client{-1};
    std::atomic<int> _mode{0};
    std::atomic<uint32_t> _settings_seq{0};
};

} // namespace services
