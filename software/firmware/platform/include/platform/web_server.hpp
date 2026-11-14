/**
 * @file
 * Abstração: o servidor web do barco.
 *
 * Entrega a página em "/", recebe o joystick por WebSocket em "/ws" e atende
 * a API REST em "/api/...". O servidor só transporta: quem entende as
 * mensagens e os pedidos é quem ouve (a camada de cima). Quem implementa na
 * placa é EspWebServer.
 */
#pragma once

// Includes
#include <cstddef> // size_t

// Defines
namespace platform {

/// @name Status HTTP que quem atende a API pode devolver
/// @{
constexpr int HTTP_OK                 = 200;
constexpr int HTTP_NO_CONTENT         = 204; ///< deu certo, sem corpo
constexpr int HTTP_BAD_REQUEST        = 400;
constexpr int HTTP_NOT_FOUND          = 404;
constexpr int HTTP_METHOD_NOT_ALLOWED = 405;
/// @}

// Enums/structs

/// Métodos HTTP que a API usa
enum class http_method_e {
    GET,   ///< ler um recurso
    PATCH, ///< mudar parte de um recurso
};

// Classes

/// Quem ouve as mensagens de texto do WebSocket implementa isto
class TextListener {
  public:
    /**
     * Chegou uma mensagem de texto pelo WebSocket.
     *
     * Roda na tarefa do servidor, não no laço principal.
     *
     * @param text   a mensagem, terminada em '\0'
     * @param length tamanho, sem o '\0'
     * @param client quem mandou (pra conferir depois se ainda está aberto)
     */
    virtual void on_text(const char *text, size_t length, int client) = 0;

  protected:
    ~TextListener() = default;
};

/// Quem atende os pedidos da API implementa isto
class RequestListener {
  public:
    /**
     * Atenda um pedido da API.
     *
     * Roda na tarefa do servidor, não no laço principal.
     *
     * @param method   GET ou PATCH
     * @param path     o caminho, sem a query (ex.: "/api/status")
     * @param body     o corpo do pedido, terminado em '\0' (vazio no GET)
     * @param response onde escrever a resposta (JSON), terminada em '\0'
     * @param size     tamanho de response
     * @return o status HTTP (HTTP_OK, HTTP_NOT_FOUND...)
     */
    virtual int on_request(
        http_method_e method,
        const char *path,
        const char *body,
        char *response,
        size_t size
    ) = 0;

  protected:
    ~RequestListener() = default;
};

/// O servidor web: página, WebSocket e API
class WebServer {
  public:
    /**
     * Suba o servidor.
     *
     * @param page        a página, que fica entregue do jeito que está
     * @param page_length tamanho da página
     * @param stream      quem ouve o WebSocket
     * @param api         quem atende a API
     * @return false se não subiu
     */
    virtual bool begin(
        const char *page,
        size_t page_length,
        TextListener &stream,
        RequestListener &api
    ) = 0;

    /// Diga se o cliente ainda está com o WebSocket aberto
    virtual bool is_client_open(int client) = 0;

  protected:
    ~WebServer() = default;
};

} // namespace platform
