# Projeto ESP-IDF do firmware da DonaTelos. Compila no Docker — os comandos
# estão no README.md desta pasta.
cmake_minimum_required(VERSION 3.22)

# Cada camada é um componente; os de components/ o ESP-IDF acha sozinho
set(EXTRA_COMPONENT_DIRS app services platform common)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(donatelos)
