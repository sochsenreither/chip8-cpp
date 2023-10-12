#pragma once

#include <string>

#include "../core/chip8.h"
#include "../core/display.h"
#include "window.h"

class Engine {
   public:
    bool init();
    void load_rom(std::string filename);
    void start();

   private:
    Chip8 chip8;
    Window window;

    void update();
    void draw();
};