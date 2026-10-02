cmake_minimum_required(VERSION 3.16)
project(gpio_systemc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontrar SystemC instalado via APT (libsystemc-dev)
find_path(SYSTEMC_INCLUDE_DIR
    NAMES systemc.h
    PATHS /usr/include /usr/local/include
)

find_library(SYSTEMC_LIBRARY
    NAMES systemc
    PATHS /usr/lib/x86_64-linux-gnu /usr/lib /usr/local/lib
)

if(NOT SYSTEMC_INCLUDE_DIR)
    message(FATAL_ERROR "SystemC headers nao encontrados!")
endif()

if(NOT SYSTEMC_LIBRARY)
    message(FATAL_ERROR "SystemC library nao encontrada!")
endif()

message(STATUS "SystemC include: ${SYSTEMC_INCLUDE_DIR}")
message(STATUS "SystemC library: ${SYSTEMC_LIBRARY}")

# Biblioteca GPIO
add_library(gpio STATIC src/gpio.cpp)
target_include_directories(gpio PUBLIC include ${SYSTEMC_INCLUDE_DIR})
target_link_libraries(gpio PUBLIC ${SYSTEMC_LIBRARY})

# Biblioteca Bridge
add_library(bridge STATIC src/bridge.cpp)
target_include_directories(bridge PUBLIC include ${SYSTEMC_INCLUDE_DIR})
target_link_libraries(bridge PUBLIC ${SYSTEMC_LIBRARY})

# Executavel principal
add_executable(gpio_sim src/main.cpp)
target_link_libraries(gpio_sim PRIVATE gpio bridge)