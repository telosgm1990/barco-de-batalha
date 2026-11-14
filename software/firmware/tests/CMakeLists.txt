# Testes no PC, sem placa: a conta pura, os componentes, o controle remoto e o
# barco — cada um com peças falsas no lugar das abstrações da camada de baixo.
# Nada aqui compila o que fala com o ESP-IDF (as concretas Esp* da platform/):
# só os headers das abstrações.
cmake_minimum_required(VERSION 3.22)
project(donatelos_tests CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(host_tests
    test_main.cpp
    ../services/src/drive.cpp
    ../services/src/protocol.cpp
    ../services/src/wifi_remote.cpp
    ../app/src/boat.cpp
    ../app/src/failsafe.cpp
    ../components/motor/src/h_bridge_motor.cpp
    ../components/led/src/digital_led.cpp
    ../components/battery/src/divider_battery.cpp
)
target_include_directories(host_tests PRIVATE
    ../common/include
    ../platform/include
    ../components/motor/include
    ../components/led/include
    ../components/battery/include
    ../services/include
    ../app/include
)
target_compile_options(host_tests PRIVATE -Wall -Wextra -Werror)
