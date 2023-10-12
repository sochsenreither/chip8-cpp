#pragma once

#include <string>

#include "display.h"
#include "keypad.h"
#include "memory.h"

class Chip8 {
   public:
    void reset();
    void load_rom(std::string filename);
    void update_delay_timer();
    bool update_sound_timer();
    void set_key(int key, int val);
    void tick();

    uint8_t get_pixel(int i);

   private:
    std::array<uint8_t, 0x10> regs = {0};
    std::array<uint16_t, 0x10> stack = {0};

    uint16_t I = {0};   // Index register
    uint16_t pc = {0};  // Program counter
    uint16_t sp = {0};  // Stack pointer
    uint8_t delay_timer = 0;
    uint8_t sound_timer = 0;

    Memory memory;
    Display display;
    Keypad keypad;

    // Instructions

    void cls();
    void ret();
    void jmp(int nnn);
    void call(int nnn);
    void se_byte(int x, int kk);
    void sne_byte(int x, int kk);
    void se_reg(int x, int y);
    void ld_byte(int x, int kk);
    void add_byte(int x, int kk);
    void ld_reg(int x, int y);
    void fn_or(int x, int y);
    void fn_and(int x, int y);
    void fn_xor(int x, int y);
    void add_reg(int x, int y);
    void sub(int x, int y);
    void shr(int x);
    void subn(int x, int y);
    void shl(int x);
    void sne(int x, int y);
    void ld(int nnn);
    void jp_reg(int nnn);
    void rnd(int x, int kk);
    void drw(int x, int y, int n);
    void skp(int x);
    void sknp(int x);
    void ld_delay_timer(int x);
    void ld_timer_wait(int x);
    void ld_delay_timer_set(int x);
    void ld_sound_timer_set(int x);
    void add_i_reg(int x);
    void set_i_reg(int x);
    void bcd(int x);
    void cpy_regs_to_mem(int x);
    void cpy_mem_to_regs(int x);
};