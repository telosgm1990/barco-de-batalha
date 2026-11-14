# Bateria lida por um divisor resistivo num pino de ADC.
idf_component_register(
    SRCS "src/battery.cpp"
    INCLUDE_DIRS "include"
    REQUIRES platform
)
