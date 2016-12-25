#ifndef L_UTILS
#define L_UTILS

#include <string>
#include <iostream>
#include <SDL2/SDL.h>

#include "types.h"
#include "constants.h"

void log_info(const std::string &msg)
{
    std::cerr << "[INFO] " << msg << std::endl;
}

void log_error(const std::string &msg)
{
    std::cerr << "[ERROR] " << msg << std::endl;
}

void log_sdl_error(const std::string &msg)
{
    std::cerr << "[SDL-ERROR] " << msg << " (" << SDL_GetError() << ")" << std::endl;
}

// TODO: Replace cout use with log use
sdl_init_result basic_init()
{
    sdl_init_result result = {nullptr, nullptr};
    // Init video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        log_sdl_error("SDL_Init failed");
        return result;
    }
    result.win = SDL_CreateWindow("Hello World!", 100, 100, DEF_WIDTH, DEF_HEIGHT, SDL_WINDOW_SHOWN);
    if (result.win == nullptr){
        log_sdl_error("SDL_CreateWindow failed");
        SDL_Quit();
        return result;
    }
    result.ren = SDL_CreateRenderer(result.win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (result.ren == nullptr){
        SDL_DestroyWindow(result.win);
        log_sdl_error("SDL_CreateRenderer failed");
        SDL_Quit();
        return result;
    }

    return result;
}

#endif /* ifndef L_UTILS */

