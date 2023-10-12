#include "engine.h"

#include <chrono>
#include <iostream>

bool Engine::init() {
    auto res_window = window.init(Display::width(), Display::height(), "Chip8");
    return res_window;
}

void Engine::load_rom(std::string filename) {
    chip8.reset();
    chip8.load_rom(filename);
}

void Engine::start() {
    auto get_time = [] { return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); };

    auto start = get_time();

    // TODO: fixed for 60 fps
    while (window.running) {
        auto delta = get_time() - start;
        if (delta > (1000.0/60.0)) {
            start = get_time();
            update();
            draw();
        }
    }
}

void Engine::update() {
    // TODO: fixed for 10 instructions per second
    auto cycles = 10;

    chip8.update_delay_timer();

    window.poll_events([&](int key, int val) {
        chip8.set_key(key, val);
    });

    for (auto i = 0; i < cycles; i++) {
        chip8.tick();
    }
}

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
