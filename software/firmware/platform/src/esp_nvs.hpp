/**
 * @file
 * A flash de configuração (NVS), que o Wi-Fi e a memória do barco dividem.
 * Header interno da platform/: ninguém de fora inclui.
 */
#pragma once

// Funções
namespace platform {

/// Prepare a NVS uma vez só (as outras chamadas só dizem se deu); se ela
/// estiver cheia ou numa versão antiga, apague e comece de novo
bool begin_nvs_flash();

} // namespace platform
