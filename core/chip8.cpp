#include "chip8.h"

#include <iostream>

void Chip8::reset() {
    memory.reset();
    keypad.reset();
    display.clear();

    regs = {0};
    stack = {0};
    I = 0;
    pc = memory.offset;
    sp = 0;
    delay_timer = 0;
    sound_timer = 0;
}

void Chip8::load_rom(std::string filename) {
    memory.load_rom(filename);
}

void Chip8::update_delay_timer() {
    if (delay_timer > 0) {
        delay_timer--;
    }
}

bool Chip8::update_sound_timer() {
    if (sound_timer > 0) {
        sound_timer--;
    }
    return sound_timer > 0;
}

void Chip8::set_key(int key, int val) {
    keypad[key] = val;
}

void Chip8::tick() {
    // Fetch
    auto opcode = memory[pc] << 8 | memory[pc + 1];

    // Decode
    auto type = (opcode >> 12) & 0x000F;
    auto n = opcode & 0x000F;
    auto x = (opcode >> 8) & 0x000F;
    auto y = (opcode >> 4) & 0x000F;
    auto kk = opcode & 0x00FF;
    auto nnn = opcode & 0x0FFF;

    // Execute
    pc += 2;

    // TODO: maybe put this in a separate function
    switch (type) {
        case 0x00:
            switch (nnn) {
                case 0xE0:
                    return cls();
                case 0xEE:
                    return ret();
                default:
                    return;
            }
        case 0x01:
            return jmp(nnn);
        case 0x02:
            return call(nnn);
        case 0x03:
            return se_byte(x, kk);
        case 0x04:
            return sne_byte(x, kk);
        case 0x05:
            return se_reg(x, y);
        case 0x06:
            return ld_byte(x, kk);
        case 0x07:
            return add_byte(x, kk);
        case 0x08:
            switch (n) {
                case 0x0:
                    return ld_reg(x, y);
                case 0x1:
                    return fn_or(x, y);
                case 0x2:
                    return fn_and(x, y);
                case 0x3:
                    return fn_xor(x, y);
                case 0x4:
                    return add_reg(x, y);
                case 0x5:
                    return sub(x, y);
                case 0x6:
                    return shr(x);
                case 0x7:
                    return subn(x, y);
                case 0xE:
                    return shl(x);
                default:
                    return;
            }
        case 0x09:
            return sne(x, y);
        case 0xA:
            return ld(nnn);
        case 0xB:
            return jp_reg(nnn);
        case 0xC:
            return rnd(x, kk);
        case 0xD:
            return drw(x, y, n);
        case 0xE:
            if (kk == 0x9E) {
                return skp(x);
            }
            if (kk == 0xA1) {
                return sknp(x);
            }
        case 0xF:
            switch (kk) {
                case 0x07:
                    return ld_delay_timer(x);
                case 0x0A:
                    return ld_timer_wait(x);
                case 0x15:
                    return ld_delay_timer_set(x);
                case 0x18:
                    return ld_sound_timer_set(x);
                case 0x1E:
                    return add_i_reg(x);
                case 0x29:
                    return set_i_reg(x);
                case 0x33:
                    return bcd(x);
                case 0x55:
                    return cpy_regs_to_mem(x);
                case 0x65:
                    return cpy_mem_to_regs(x);
                default:
                    return;
            }
        default:
            return;
    }
}

uint8_t Chip8::get_pixel(int i) {
    return display[i];
}

// CLS: Clear the display.
void Chip8::cls() {
    // std::cout << __func__ << std::endl;
    display.clear();
}

// RET: Return from a subroutine.
// The interpreter sets the program counter to the address at the top of the stack,
// then subtracts 1 from the stack pointer.
void Chip8::ret() {
    // std::cout << __func__ << std::endl;
    pc = stack[--sp];
}

// JP addr: Jump to location nnn.
// The interpreter sets the program counter to nnn.
void Chip8::jmp(int nnn) {
    // std::cout << __func__ << std::endl;
    pc = nnn;
}

// CALL addr: Call subroutine at nnn.
// The interpreter increments the stack pointer, then puts the current PC on the top of the
// stack. The PC is then set to nnn.
void Chip8::call(int nnn) {
    // std::cout << __func__ << std::endl;
    stack[sp++] = pc;
    pc = nnn;
}

// SE Vx, byte: Skip next instruction if Vx == kk.
// The interpreter compares register Vx to kk, and if they are equal, increments the program
// counter by 2.
void Chip8::se_byte(int x, int kk) {
    // std::cout << __func__ << std::endl;
    if (regs[x] == kk) {
        pc += 2;
    }
}

// SNE Vx, byte: Skip next instruction if Vx != kk.
// The interpreter compares register Vx to kk, and if they are not equal, increments the
// program counter by 2.
void Chip8::sne_byte(int x, int kk) {
    // std::cout << __func__ << std::endl;
    if (regs[x] != kk) {
        pc += 2;
    }
}

// SE Vx, Vy: Skip next instruction if Vx == Vy.
// The interpreter compares register Vx to register Vy, and if they are equal, increments the
// program counter by 2.
void Chip8::se_reg(int x, int y) {
    // std::cout << __func__ << std::endl;
    if (regs[x] == regs[y]) {
        pc += 2;
    }
}

// LD Vx, byte: Set Vx = kk.
// The interpreter puts the value kk into register Vx.
void Chip8::ld_byte(int x, int kk) {
    // std::cout << __func__ << std::endl;
    regs[x] = kk;
}

// ADD Vx, byte: Set Vx = Vx + kk.
// Adds the value kk to the value of register Vx, then stores the result in Vx.
void Chip8::add_byte(int x, int kk) {
    // std::cout << __func__ << std::endl;
    regs[x] += kk;
}

// LD Vx, Vy: Set Vx = Vy.
// Stores the value of register Vy in register Vx.
void Chip8::ld_reg(int x, int y) {
    // std::cout << __func__ << std::endl;
    regs[x] = regs[y];
}

// OR Vx, Vy: Set Vx = Vx OR Vy.
// Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
void Chip8::fn_or(int x, int y) {
    // std::cout << __func__ << std::endl;
    regs[x] |= regs[y];
}

// AND Vx, Vy: Set Vx = Vx AND Vy.
// Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
void Chip8::fn_and(int x, int y) {
    // std::cout << __func__ << std::endl;
    regs[x] &= regs[y];
}

// XOR Vx, Vy: Set Vx = Vx XOR Vy.
// Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx.
void Chip8::fn_xor(int x, int y) {
    // std::cout << __func__ << std::endl;
    regs[x] ^= regs[y];
}

// ADD Vx, Vy: Set Vx = Vx + Vy, set VF = carry.
// The values of Vx and Vy are added together.
// If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0.
// Only the lowest 8 bits of the result are kept, and stored in Vx.
void Chip8::add_reg(int x, int y) {
    // std::cout << __func__ << std::endl;
    regs[0xF] = ((unsigned)regs[x] + (unsigned)regs[y] > 0xFF) ? 1 : 0;
    regs[x] += regs[y];
}

// SUB Vx, Vy: Set Vx = Vx - Vy, set VF = NOT borrow.
// If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the
// results stored in Vx.
void Chip8::sub(int x, int y) {
    // std::cout << __func__ << std::endl;
    regs[0xF] = (regs[x] > regs[y]) ? 1 : 0;
    regs[x] -= regs[y];
}

// SHR Vy {, Vy}: Set Vx = Vx SHR 1.
// If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is
// divided by 2.
void Chip8::shr(int x) {
    // std::cout << __func__ << std::endl;
    regs[0xF] = regs[x] & 1;
    regs[x] >>= 1;
}

// SUBN Vx, Vy: Set Vx = Vy - Vx, set VF = NOT borrow.
// If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the
// results stored in Vx.
void Chip8::subn(int x, int y) {
    // std::cout << __func__ << std::endl;
    regs[0xF] = (regs[y] > regs[x]) ? 1 : 0;
    regs[x] = regs[y] - regs[x];
}

// SHL Vx {, Vy}: Set Vx = Vx SHL 1.
// If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0.
// Then Vx is multiplied by 2.
void Chip8::shl(int x) {
    // std::cout << __func__ << std::endl;
    regs[0xF] = (regs[x] >> 7) & 0x1;
    regs[x] <<= 1;
}

// SNE Vx, Vy: Skip next instruction if Vx != Vy.
// The values of Vx and Vy are compared, and if they are not equal, the program counter
// is increased by 2.
void Chip8::sne(int x, int y) {
    // std::cout << __func__ << std::endl;
    if (regs[x] != regs[y]) {
        pc += 2;
    }
}

// LD i, addr: Set I = nnn.
// The value of register I is set to nnn.
void Chip8::ld(int nnn) {
    // std::cout << __func__ << std::endl;
    I = nnn;
}

// JP V0, addr: Jump to location nnn + V0.
// The program counter is set to nnn plus the value of V0.
void Chip8::jp_reg(int nnn) {
    // std::cout << __func__ << std::endl;
    pc = nnn + regs[0];
}

// RND Vx, byte: Set Vx = random byte AND kk.
// The interpreter generates a random number from 0 to 255, which is then ANDed with
// the value kk. The results are stored in Vx.
void Chip8::rnd(int x, int kk) {
    // std::cout << __func__ << std::endl;
    // TODO: implement me
    regs[x] = 1 & kk;
}

// DRW Vx, Vy, nibble: Display n-byte sprite starting at memory location I at (Vx, Vy),
// set VF = collision.
// The interpreter reads n bytes from memory, starting at the address stored in I.
// These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
// Sprites are XORed onto the existing screen. If this causes any pixels to be erased,
// VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is
// outside the coordinates of the display, it wraps around to the opposite side of the screen.
void Chip8::drw(int x, int y, int n) {
    // std::cout << __func__ << std::endl;
    regs[0xF] = 0;

    for (auto row = 0; row < n; row++) {
        auto sprite = memory[I + row];
        for (auto col = 0; col < 8; col++) {
            if (sprite & (0b10000000 >> col)) {
                auto px = regs[x] + col;
                auto py = regs[y] + row;
                auto pixel = px + py * display.m_width;
                if (display[pixel]) {
                    regs[0x0F] = 1;
                }
                display[pixel] ^= 1;
            }
        }
    }
}

// SKP Vx: Skip next instruction if key with the value of Vx is pressed.
// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the
// down position, PC is increased by 2.
void Chip8::skp(int x) {
    // std::cout << __func__ << std::endl;
    if (keypad.is_pressed(regs[x]) == 1) {
        pc += 2;
    }
}

// SKNP Vx: Skip next instruction if key with the value of Vx is not pressed.
// Checks the keyboard, and if the key corresponding to the value of Vx is currently in
// the up position, PC is increased by 2.
void Chip8::sknp(int x) {
    // std::cout << __func__ << std::endl;
    if (keypad.is_pressed(regs[x]) == 0) {
        pc += 2;
    }
}

// LD Vx, DT: Set Vx = delay timer value.
// The value of DT is placed into Vx.
void Chip8::ld_delay_timer(int x) {
    // std::cout << __func__ << std::endl;
    regs[x] = delay_timer;
}

// Ld Vx, K: Wait for a key press, store the value of the key in Vx.
// All execution stops until a key is pressed, then the value of that key is stored in Vx.
void Chip8::ld_timer_wait(int x) {
    // std::cout << __func__ << std::endl;
    for (auto i = 0; i < 0x0F; i++) {
        if (keypad.is_pressed(i)) {
            regs[x] = i;
        }
    }
}

// LD DT, Vx: Set delay timer = Vx.
// DT is set equal to the value of Vx.
void Chip8::ld_delay_timer_set(int x) {
    // std::cout << __func__ << std::endl;
    delay_timer = regs[x];
}

// LD ST, Vx: Set sound timer = Vx.
// ST is set equal to the value of Vx.
void Chip8::ld_sound_timer_set(int x) {
    // std::cout << __func__ << std::endl;
    sound_timer = regs[x];
}

// ADD I, Vx: Set I = I + Vx.
// The values of I and Vx are added, and the results are stored in I.
void Chip8::add_i_reg(int x) {
    // std::cout << __func__ << std::endl;
    // TODO: overflow flag?
    I += regs[x];
}

// LD F, Vx: Set I = location of sprite for digit Vx.
// The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.
void Chip8::set_i_reg(int x) {
    // std::cout << __func__ << std::endl;
    I = regs[x] * 5;
}

// LD B, Vx: Store BCD representation of Vx in memory locations I, I+1, and I+2.
// The interpreter takes the decimal value of Vx, and places the hundreds digit in memory
// at location in I, the tens digit at location I+1, and the ones digit at location I+2.
void Chip8::bcd(int x) {
    // std::cout << __func__ << std::endl;
    memory[(I + 0) & 0xFFF] = (regs[x] % 1000) / 100;
    memory[(I + 1) & 0xFFF] = (regs[x] % 100) / 10;
    memory[(I + 2) & 0xFFF] = regs[x] % 10;
}

// LD [I], Vx: Store registers V0 through Vx in memory starting at location I.
// The interpreter copies the values of registers V0 through Vx into memory, starting at
// the address in I.
void Chip8::cpy_regs_to_mem(int x) {
    // std::cout << __func__ << std::endl;
    for (auto index = 0; index < x; index++) {
        memory[I++ & 0xFFF] = regs[index];
    }
}

// LD Vx, [I]: Read registers V0 through Vx from memory starting at location I.
// The interpreter reads values from memory starting at location I into registers V0 through Vx.
void Chip8::cpy_mem_to_regs(int x) {
    // std::cout << __func__ << std::endl;
    for (auto index = 0; index < x; index++) {
        regs[index] = memory[I++ & 0xFFF];
    }
}