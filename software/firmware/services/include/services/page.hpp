/**
 * @file
 * A página do joystick, embutida no binário pelo EMBED_TXTFILES do
 * CMakeLists.txt de services/. Só o main/ inclui isto, pra entregar a página
 * pro WifiRemote — os testes do PC passam outra.
 */
#pragma once

// Funções
namespace services {

/// Início da página; o nome do símbolo vem do linker
extern const char page_start[] asm("_binary_index_html_start");

/// Fim da página (o EMBED_TXTFILES põe um '\0' antes dele)
extern const char page_end[] asm("_binary_index_html_end");

} // namespace services
