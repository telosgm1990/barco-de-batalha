# services/: tração (joystick -> dois motores), protocolo da página e o
# controle remoto pelo Wi-Fi. drive e protocol são conta pura — rodam nos
# testes do PC.
idf_component_register(
    SRCS
        "src/drive.cpp"
        "src/protocol.cpp"
        "src/remote.cpp"
    INCLUDE_DIRS "include"
    REQUIRES platform common
    EMBED_TXTFILES "page/index.html"
)
