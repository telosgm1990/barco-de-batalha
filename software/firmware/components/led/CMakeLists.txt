# LED de status: a abstração (led.hpp) e o LED numa saída digital que a
# implementa (digital_led.hpp). Depende só da abstração de saída da platform/.
idf_component_register(
    SRCS "src/digital_led.cpp"
    INCLUDE_DIRS "include"
    REQUIRES platform
)
