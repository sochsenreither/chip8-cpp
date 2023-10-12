#include "engine/engine.h"

#include <iostream>

int main() {
    std::string filepath = "roms/TETRIS";

    Engine engine;

    if (!engine.init()) {
        std::cerr << "Failed to initialize engine" << std::endl;
        return EXIT_FAILURE;
    }

    engine.load_rom(filepath);
    engine.start();

    return EXIT_SUCCESS;
}