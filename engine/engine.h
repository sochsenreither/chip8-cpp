#pragma once

#include <string>

#include "../core/chip8.h"
#include "../core/display.h"
#include "window.h"

class Engine {
   public:
    [[nodiscard]] bool init(int cycles = 10, float fps = 60.0);
    void load_rom(std::string filename);
    void start();

   private:
    int cycles_per_second;
    float frames_per_second;
    Chip8 chip8;
    Window window;

    void update();
    void draw();
};