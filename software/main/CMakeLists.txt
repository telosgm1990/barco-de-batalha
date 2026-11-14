# main/: app_main e o bootstrap — a cola com a placa (pinos e peças).
idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "."
    REQUIRES app services motor battery led platform
)
