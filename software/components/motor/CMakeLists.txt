# Motor DC numa ponte H de duas entradas (TC1508A, MX1508 e afins).
idf_component_register(
    SRCS "src/motor.cpp"
    INCLUDE_DIRS "include"
    REQUIRES platform common
)
