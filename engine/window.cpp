#include "window.h"

#include <iostream>

Window::~Window() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

// Sets up a SDL window.
bool Window::init(int width, int height, std::string title) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
    if (window == nullptr || renderer == nullptr) {
        std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetWindowTitle(window, title.data());
    running = true;

    return true;
}

// Poll for input.
void Window::poll_events(Chip8* chip8) {
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
            chip8->set_key(event.key.keysym.sym, 1);
        }
        if (event.type == SDL_KEYUP) {
            chip8->set_key(event.key.keysym.sym, 0);
        }
    }
}

void Window::render() {
    SDL_RenderPresent(renderer);
}

void Window::clear_screen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(renderer);
}

void Window::draw_pixel(int x, int y, int scale) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_Rect rect{x * scale, y * scale, scale, scale};
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);
}
