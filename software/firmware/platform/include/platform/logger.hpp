/**
 * @file
 * Abstração: log. A camada de cima chama info/warn, no estilo printf; quem
 * escreve de verdade é a concreta (EspLogger, na placa) via write().
 */
#pragma once

// Includes
#include <cstdarg> // va_list
#include <cstdio>  // vsnprintf

// Enums/structs
namespace platform {

/// Nível da linha de log
enum class log_level_e {
    INFO, ///< andamento normal
    WARN  ///< algo deu errado, mas o barco segue
};

// Classes

/// O log: a camada de cima formata, a concreta escreve
class Logger {
  public:
    /// Escreva uma linha pronta, no nível pedido
    virtual void write(
        log_level_e level,
        const char *tag,
        const char *text
    ) = 0;

    /// Escreva uma linha de informação, estilo printf
    [[gnu::format(printf, 3, 4)]] void info(
        const char *tag,
        const char *format,
        ...
    ) {
        va_list args;
        va_start(args, format);
        _format(log_level_e::INFO, tag, format, args);
        va_end(args);
    }

    /// Escreva uma linha de aviso, estilo printf
    [[gnu::format(printf, 3, 4)]] void warn(
        const char *tag,
        const char *format,
        ...
    ) {
        va_list args;
        va_start(args, format);
        _format(log_level_e::WARN, tag, format, args);
        va_end(args);
    }

  protected:
    ~Logger() = default;

  private:
    static constexpr int LINE_SIZE = 160;

    void _format(
        log_level_e level,
        const char *tag,
        const char *format,
        va_list args
    ) {
        char line[LINE_SIZE];
        vsnprintf(line, sizeof(line), format, args);
        write(level, tag, line);
    }
};

} // namespace platform
