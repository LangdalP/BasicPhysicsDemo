
#ifndef L_LOGGING
#define L_LOGGING 

#include <string>
#include <stdio.h>
#include <SDL2/SDL_error.h>

inline void log_info(const std::string &msg)
{
    printf("[INFO] %s\n", msg.c_str());
}

inline void log_error(const std::string &msg)
{
    fprintf(stderr, "[ERROR] %s\n", msg.c_str());
}

#endif /* ifndef L_LOGGING */
