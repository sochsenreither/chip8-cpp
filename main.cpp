#include <iostream>

#include "engine/engine.h"

int main(int argc, char** argv) {
    std::string filepath;
    int cycles = 10;
    float fps = 60.0;
    if (argc < 2) {
        std::cout << "No rom provided, loading TETRIS..." << std::endl;
        filepath = "roms/TETRIS";
    } else {
        filepath = argv[1];
    }

    Engine engine;

    if (!engine.init(cycles, fps)) {
        std::cerr << "Failed to initialize engine" << std::endl;
        return EXIT_FAILURE;
    }

    engine.load_rom(filepath);
    engine.start();

    return EXIT_SUCCESS;
}