#include "memory.h"

#include <fstream>

void Memory::reset() {
    memory.fill(0);
    load_sprites();
}

// Copys the sprites into the memory.
void Memory::load_sprites() {
    std::copy(begin(sprites), end(sprites), begin(memory));
}

// Loads a rom into memory starting at the offset.
void Memory::load_rom(std::string filename) {
    // Check for valid file path
    std::ifstream file(filename);
    if (!file.good()) {
        throw std::runtime_error("Invalid ROM path!\n");
    }

    // Copy file content into memory
    auto start_addr = offset;
    for (std::ifstream file(filename, std::ios::binary); file.good();) {
        if (start_addr >= 0x1000) {
            throw std::runtime_error("Out of memory!\n");
        }
        memory[start_addr++] = file.get();
    }
}

uint8_t& Memory::operator[](int index) {
    return memory.at(index);
}
