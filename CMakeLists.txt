cmake_minimum_required(VERSION 3.21)
project(chip8)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# enable_testing()

add_subdirectory(vendor/SDL)
# add_subdirectory(vendor/googletest)
# add_subdirectory(tests)

set(HEADERS
    core/chip8.h
    core/keypad.h
    core/memory.h
    core/display.h
    engine/engine.h
    engine/window.h
)

set(SOURCES
    main.cpp
    core/chip8.cpp
    core/memory.cpp
    engine/engine.cpp
    engine/window.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(chip8 SDL2-static)
