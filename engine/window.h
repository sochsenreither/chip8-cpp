#pragma once

#include <SDL.h>
#include "../core/chip8.h"

#include <string>

class Window {
   public:
    bool running = false;

    ~Window();

    [[nodiscard]] bool init(int width, int height, std::string title);

    void poll_events(Chip8* chip8);
    void render();
    void clear_screen();
    void draw_pixel(int x, int y, int scale);

   private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
};