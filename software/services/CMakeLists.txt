# services/: a tração (Drive, conta pura), o protocolo da página e o controle
# remoto — a abstração (remote.hpp) e quem a implementa pelo Wi-Fi
# (wifi_remote.hpp), em cima das abstrações da platform/.
idf_component_register(
    SRCS
        "src/drive.cpp"
        "src/protocol.cpp"
        "src/wifi_remote.cpp"
    INCLUDE_DIRS "include"
    REQUIRES platform common
    EMBED_TXTFILES "page/index.html"
)
