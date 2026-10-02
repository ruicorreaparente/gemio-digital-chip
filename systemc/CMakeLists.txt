cmake_minimum_required(VERSION 3.16)
project(gpio_systemc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SystemCLanguage CONFIG REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SystemC_CXX_FLAGS}")

# Biblioteca GPIO
add_library(gpio STATIC src/gpio.cpp)
target_include_directories(gpio PUBLIC include)
target_link_libraries(gpio PUBLIC SystemC::systemc)

# Executavel principal
add_executable(gpio_sim src/main.cpp)
target_link_libraries(gpio_sim PRIVATE gpio)