# app/: o barco — o laço que junta controle remoto, tração, motores, bateria
# e LED, todos pelas abstrações das camadas de baixo —, o failsafe e a vigia
# da bateria.
idf_component_register(
    SRCS
        "src/battery_watch.cpp"
        "src/boat.cpp"
        "src/failsafe.cpp"
    INCLUDE_DIRS "include"
    REQUIRES services motor battery led platform common
)
