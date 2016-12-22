#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <vector>
#include <stdlib.h>

#include "resources.h"
#include "constants.h"
#include "utils.h"

using std::cout;
using std::endl;

#define PI 3.14159265

// TODO: This is not ECS-like
struct l_circle {
    Sint16 x;
    Sint16 y;
    Sint16 dx;
    Sint16 dy;
    Sint16 rad;
};

std::vector<l_circle> circles;

void init_circles(int count)
{
    srand(1250424);
    for (int i = 0; i < count; ++i) {
        // Circle
        l_circle circle;
        circle.x = rand() % DEF_WIDTH;
        circle.y = rand() % DEF_HEIGHT;
        circle.dx = (rand() % 4) - 2;
        circle.dy = (rand() % 4) - 2;
        circle.rad = 30;
        circles.push_back(circle);
    }
}

void drawCircles(SDL_Renderer *ren, int iteration)
{
    for (auto &circle : circles) {
        filledCircleRGBA(ren, circle.x, circle.y, circle.rad, 255, 255, 255, 255);
        circle.x += circle.dx;
        circle.y += circle.dy;
    }

}

int main(int argc, char *argv[])
{
    cout << "Hello, SDL world!" << endl;

    sdl_init_result sdl = basic_init();
    if (sdl.win == nullptr || sdl.ren == nullptr) {
        return -1;
    }

    init_circles(10);

    SDL_Event e;

    bool quit = false;
    int iteration_counter = 0;
    while (!quit && iteration_counter < 500) {
        if (SDL_PollEvent(&e)) 
        { 
            if (e.type == SDL_QUIT) 
                quit = true;
        } 
        //First clear the renderer
        SDL_SetRenderDrawColor(sdl.ren, 20, 20, 20, 0);
        SDL_RenderClear(sdl.ren);
        //Draw circles
        drawCircles(sdl.ren, iteration_counter);
        //Update the screen
        SDL_RenderPresent(sdl.ren);
        //Take a quick break after all that hard work
        SDL_Delay(16);
        iteration_counter++;
    }

    SDL_DestroyRenderer(sdl.ren);
    SDL_DestroyWindow(sdl.win);
    SDL_Quit();

    return 0;
}
