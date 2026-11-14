# app/: o barco — o laço que junta controle remoto, tração, motores, bateria
# e LED, todos pelas abstrações das camadas de baixo — e o failsafe.
idf_component_register(
    SRCS
        "src/boat.cpp"
        "src/failsafe.cpp"
    INCLUDE_DIRS "include"
    REQUIRES services motor battery led platform common
)
