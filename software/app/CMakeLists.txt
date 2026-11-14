# app/: o barco — o laço que junta controle remoto, tração, motores,
# bateria e LED — e o failsafe, que é conta pura e roda nos testes do PC.
idf_component_register(
    SRCS
        "src/boat.cpp"
        "src/failsafe.cpp"
    INCLUDE_DIRS "include"
    REQUIRES services motor battery led platform common
)
