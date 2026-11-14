/*
 * Log na serial (a USB da placa), no formato de sempre do ESP-IDF.
 */
#pragma once

// Funções
namespace platform::log {

// Escreva uma linha de informação, estilo printf
void info(const char *tag, const char *format, ...)
    __attribute__((format(printf, 2, 3)));

// Escreva uma linha de aviso, estilo printf
void warn(const char *tag, const char *format, ...)
    __attribute__((format(printf, 2, 3)));

} // namespace platform::log
