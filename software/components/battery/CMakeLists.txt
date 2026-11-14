# Bateria: a abstração (battery.hpp) e a leitura por divisor resistivo que a
# implementa (divider_battery.hpp). Depende só da abstração de entrada
# analógica da platform/.
idf_component_register(
    SRCS "src/divider_battery.cpp"
    INCLUDE_DIRS "include"
    REQUIRES platform
)
