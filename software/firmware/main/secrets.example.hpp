/**
 * @file
 * Nome e senha da rede Wi-Fi que o barco cria — exemplo.
 *
 * Copie pra secrets.hpp (que o git ignora) e troque a senha. Sem o
 * secrets.hpp, o firmware compila com este exemplo e avisa no build.
 */
#pragma once

// Defines
namespace secrets {

constexpr const char *WIFI_SSID     = "DonaTelos";         ///< nome da rede
constexpr const char *WIFI_PASSWORD = "troque-esta-senha"; ///< 8+ caracteres

} // namespace secrets
