cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)
include(pico_sdk_import.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(snake C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

# SSD1306 driver lives next to this project (../SSD1306). It's outside this
# source tree, so add_subdirectory() needs an explicit binary dir.
set(SSD1306_BUILD_EXAMPLES OFF)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../SSD1306 ${CMAKE_BINARY_DIR}/SSD1306)

# Embedded Template Library (header-only, fixed-capacity containers, no heap).
include(FetchContent)
FetchContent_Declare(etl
    GIT_REPOSITORY https://github.com/ETLCPP/etl.git
    GIT_TAG        20.49.0
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(etl)

file(GLOB SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    pico_stdlib
    hardware_i2c
    ssd1306::ssd1306
    etl::etl
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_add_extra_outputs(${PROJECT_NAME})
