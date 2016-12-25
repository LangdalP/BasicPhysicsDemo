
#ifndef L_LOGGING
#define L_LOGGING 

#include <string>
#include <stdio.h>
#include <SDL2/SDL_error.h>

void log_info(const std::string &msg)
{
    printf("[INFO] %s\n", msg.c_str());
}

void log_error(const std::string &msg)
{
    fprintf(stderr, "[ERROR] %s\n", msg.c_str());
}

void log_sdl_error(const std::string &msg)
{
    fprintf(stderr, "[SDL-ERROR] %s ( %s )\n", msg.c_str(), SDL_GetError());
}

#endif /* ifndef L_LOGGING */
