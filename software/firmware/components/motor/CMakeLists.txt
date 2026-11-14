# Motor: a abstração (motor.hpp) e a ponte H de duas entradas que a implementa
# (h_bridge_motor.hpp). Depende só da abstração de PWM da platform/.
idf_component_register(
    SRCS "src/h_bridge_motor.cpp"
    INCLUDE_DIRS "include"
    REQUIRES platform common
)
