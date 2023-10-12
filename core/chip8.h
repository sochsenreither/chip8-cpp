#pragma once

#include <string>

#include "cpu.h"
#include "display.h"
#include "keypad.h"
#include "memory.h"

class Chip8 {
   public:
    void reset();
    void load_rom(std::string filename);
    void update_delay_timer();
    void update_sound_timer();
    void tick();

   private:
    Cpu cpu;
    Memory memory;
    Display display;
    Keypad keypad;
};