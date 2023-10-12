#include "engine.h"

#include <chrono>
#include <iostream>

// Initializes a SDL window.
bool Engine::init(int cycles, float fps) {
    cycles_per_second = cycles;
    frames_per_second = fps;
    auto res_window = window.init(Display::width(), Display::height(), "Chip8");
    return res_window;
}

// Loads a rom
void Engine::load_rom(std::string filename) {
    chip8.reset();
    chip8.load_rom(filename);
}

// Starts the emulator. Frames per second are currently fixed to 60.
void Engine::start() {
    auto get_time = [] { return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); };

    auto start = get_time();

    while (window.running) {
        auto delta = get_time() - start;
        if (delta > (1000.0 / frames_per_second)) {
            start = get_time();
            update();
            draw();
        }
    }
}

// Updates the internal chip8 state, polls for keyboard input and then runs 10 cpu cycles.
void Engine::update() {
    chip8.update_delay_timer();

    window.poll_events(&chip8);

    for (auto i = 0; i < cycles_per_second; i++) {
        chip8.tick();
    }
}

// Draws every pixel of the window that needs to be drawn.
void Engine::draw() {
    window.clear_screen();
    for (auto y = 0; y < Display::m_height; y++) {
        for (auto x = 0; x < Display::m_width; x++) {
            if (chip8.get_pixel(x + (y * Display::m_width))) {
                window.draw_pixel(x, y, Display::scale);
            }
        }
    }
    window.render();
}
