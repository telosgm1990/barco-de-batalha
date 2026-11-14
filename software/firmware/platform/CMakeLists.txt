# platform/: a única camada que conhece o ESP-IDF. Cada peça tem a abstração
# (digital_output.hpp...) e a concreta Esp (esp_digital_output.hpp...), as duas
# aqui. A camada de cima só inclui as abstrações; quem monta as concretas é o
# main/. Os headers não expõem tipo do ESP-IDF.
idf_component_register(
    SRCS
        "src/esp_access_point.cpp"
        "src/esp_analog_input.cpp"
        "src/esp_clock.cpp"
        "src/esp_digital_output.cpp"
        "src/esp_key_value_store.cpp"
        "src/esp_logger.cpp"
        "src/esp_nvs.cpp"
        "src/esp_pwm_output.cpp"
        "src/esp_web_server.cpp"
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
