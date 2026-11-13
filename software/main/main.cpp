/*
 * Ponto de entrada do firmware da DonaTelos.
 *
 * Por enquanto só prova que a cadeia toda funciona: compila no Docker, grava
 * pelo esptool e dá sinal de vida na serial. As camadas (app, services,
 * components, platform, common) nascem com o firmware da Fase 1.
 */

// Includes
#include "esp_log.h" // ESP_LOGI

// Constantes/variáveis estáticas
static const char *TAG = "main";

// Ponto de entrada
extern "C" void app_main(void) {
    // Avise na serial que o barco ligou
    ESP_LOGI(TAG, "DonaTelos acordou. Devagar e sempre.");
}
