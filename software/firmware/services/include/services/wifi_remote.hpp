/**
 * @file
 * Controle remoto pelo celular — implementa Remote.
 *
 * O barco cria a rede, serve a página, recebe o joystick pelo WebSocket e
 * atende a API REST (ver protocol.hpp). Tudo o que ele usa da placa chega
 * pronto, por referência, como as abstrações da platform/: a rede, o
 * servidor, o relógio e o log.
 *
 * **Duas tarefas, um objeto.** O servidor web roda na tarefa dele (do
 * ESP-IDF) e chama on_text e on_request; o barco roda no laço principal e
 * chama get_command, get_settings e publish_status. O que as duas dividem
 * fica em `std::atomic`: cada valor é lido e escrito inteiro, sem trava. Um
 * retrato pode misturar um campo novo com um velho por uma volta do laço —
 * pra joystick e telemetria, tudo bem.
 */
#pragma once

// Includes
#include <atomic>  // std::atomic
#include <cstddef> // size_t
#include <cstdint> // int32_t, uint32_t

#include "platform/access_point.hpp" // AccessPoint
#include "platform/clock.hpp"        // Clock
#include "platform/logger.hpp"       // Logger
#include "platform/web_server.hpp"   // WebServer, TextListener, RequestListener
#include "services/remote.hpp"       // Remote

// Classes
namespace services {

/// Controle remoto pelo celular: Wi-Fi, página, WebSocket e API REST
class WifiRemote final : public Remote,
                         private platform::TextListener,
                         private platform::RequestListener {
  public:
    /**
     * Receba as peças da placa e a página.
     *
     * @param page_start início da página embutida no firmware
     * @param page_end   fim da página (pode ter um '\0', que não vai pro
     *                   navegador)
     */
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

    /// Suba a rede e o servidor
    bool begin() override;

    /// Último joystick que chegou
    command_st get_command() override;

    /// Últimos ajustes pedidos pela API
    settings_st get_settings() override;

    /// Diga se tem um celular com o WebSocket aberto
    bool is_connected() override;

    /// Guarde o retrato do barco, que o GET /api/status entrega
    void publish_status(const status_st &status) override;

  private:
    /// Chegou mensagem pelo WebSocket: guarde o joystick
    void on_text(const char *text, size_t length, int client) override;

    /// Chegou pedido da API: responda ou guarde o ajuste
    int on_request(
        platform::http_method_e method,
        const char *path,
        const char *body,
        char *response,
        size_t size
    ) override;

    /// Atenda o PATCH /api/settings
    int _patch_settings(const char *body, char *response, size_t size);

    /// Junte o retrato guardado
    status_st _load_status() const;

    platform::AccessPoint &_access_point;
    platform::WebServer &_server;
    platform::Clock &_clock;
    platform::Logger &_logger;
    const char *_page_start;
    const char *_page_end;

    // Joystick: escrito pelo servidor, lido pelo laço. x e y vão juntos num
    // inteiro só, pra nunca ler o x de um comando com o y de outro
    std::atomic<int32_t> _packed_xy{0};
    std::atomic<uint32_t> _received_ms{0};
    std::atomic<bool> _valid{false};
    std::atomic<int> _client{-1};

    // Ajustes pedidos: escritos pelo servidor, lidos pelo laço
    std::atomic<int> _mode{0};
    std::atomic<uint32_t> _mode_seq{0};
    std::atomic<int> _trim_percent{0};
    std::atomic<uint32_t> _trim_seq{0};

    // Retrato do barco: escrito pelo laço, lido pelo servidor. Começa com o
    // failsafe armado, que é como o barco liga
    std::atomic<int32_t> _status_mv{0};
    std::atomic<bool> _status_failsafe{true};
    std::atomic<bool> _status_low{false};
    std::atomic<int> _status_mode{0};
    std::atomic<int> _status_trim_percent{0};
};

} // namespace services
