# platform/: a única camada que conhece o ESP-IDF. Os headers daqui não
# expõem tipo do ESP-IDF — por isso o resto do firmware testa no PC.
idf_component_register(
    SRCS
        "src/adc.cpp"
        "src/clock.cpp"
        "src/gpio.cpp"
        "src/http.cpp"
        "src/log.cpp"
        "src/pwm.cpp"
        "src/wifi.cpp"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES
        esp_adc
        esp_driver_gpio
        esp_driver_ledc
        esp_event
        esp_http_server
        esp_netif
        esp_timer
        esp_wifi
        nvs_flash
)
