#pragma once

#include <array>

class Keypad {
   public:
    uint8_t& operator[](uint8_t index) {
        return keyboard.at(index);
    }

    bool is_pressed(uint8_t key) {
        return keyboard.at(keymap.at(key)) == 1;
    }

    void reset() {
        keyboard.fill(0);
    }

   private:
    std::array<uint8_t, 0xFF> keyboard = {0};
    std::array<uint8_t, 0x10> keymap = {
        0x78,  // X (C8: 0)
        0x31,  // 1 (C8: 1)
        0x32,  // 2 (C8: 2)
        0x33,  // 3 (C8: 3)
        0x71,  // Q (C8: 4)
        0x77,  // W (C8: 5)
        0x65,  // E (C8: 6)
        0x61,  // A (C8: 7)
        0x73,  // S (C8: 8)
        0x64,  // D (C8: 9)
        0x7a,  // Z (C8: A)
        0x63,  // C (C8: B)
        0x34,  // 4 (C8: C)
        0x72,  // R (C8: D)
        0x66,  // F (C8: E)
        0x76,  // V (C8: F)
    };
};