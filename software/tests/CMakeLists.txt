# Testes no PC, sem placa: só o que é conta pura (tração, protocolo,
# failsafe). Nada aqui inclui o ESP-IDF — é a regra das camadas que deixa.
cmake_minimum_required(VERSION 3.22)
project(donatelos_tests CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(host_tests
    test_main.cpp
    ../services/src/drive.cpp
    ../services/src/protocol.cpp
    ../app/src/failsafe.cpp
)
target_include_directories(host_tests PRIVATE
    ../common/include
    ../services/include
    ../app/include
)
target_compile_options(host_tests PRIVATE -Wall -Wextra -Werror)
