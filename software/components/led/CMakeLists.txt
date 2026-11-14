# LED de status da placa.
idf_component_register(
    SRCS "src/led.cpp"
    INCLUDE_DIRS "include"
    REQUIRES platform
)
