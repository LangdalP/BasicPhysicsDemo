#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>       // For sin/cos
#include <vector>       // For vec (circles)
#include <stdlib.h>     // For rand
#include <algorithm>    // For min

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
    float dx;
    float dy;
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
        circle.dx = ( rand()/(float)RAND_MAX ) - 0.5;
        circle.dy = ( rand()/(float)RAND_MAX ) - 0.5;
        circle.rad = 30;
        circles.push_back(circle);
    }
}

void updateCircles(uint32_t t, uint32_t dt)
{
    for (auto &circle : circles) {
        circle.x += (int16_t)(dt*circle.dx);
        circle.y += (int16_t)(dt*circle.dy);
    }
}

void drawCircles(SDL_Renderer *ren)
{
    for (auto &circle : circles) {
        filledCircleRGBA(ren, circle.x, circle.y, circle.rad, 255, 255, 255, 255);
    }
}

/*
 * Terminology: Timestep = the dt used for world simulation
 * Idea: Don't want it to "run free" because a variable timestep will give an inconsistent feel
 *      Furthermore, really big timesteps can break simulations
 * How:
 *  - Semi-fixed timestep (rendering and world updates in lock-step)
 *  - Time-consuming world (render-loop "produces" time, world/physics consumes it in discrete steps)
 *      * Frames will have some "left-over" time that is not simulated, giving temporal aliasing
 *      * Solution: Interpolate previous world state and next
 *
 */

void main_loop(SDL_Renderer *ren)
{
    init_circles(10);

    SDL_Event e;
    uint32_t t = 0;
    uint32_t dt = 10;   // Aiming for 100 fps
    uint32_t current_time = SDL_GetTicks();

    bool quit = false;
    while ( !quit ) {
        // Event handling
        if (SDL_PollEvent(&e)) 
        { 
            if (e.type == SDL_QUIT) 
                quit = true;
        } 
        
        // World update
        uint32_t new_time = SDL_GetTicks();
        uint32_t frame_time = new_time - current_time;
        current_time = new_time;
        while ( frame_time > 0 ) {
            uint32_t delta_time = std::min( frame_time, dt );
            // Update world here
            updateCircles(t, delta_time);
            frame_time -= delta_time;
            t += delta_time;
        }

        // Rendering
        SDL_SetRenderDrawColor(ren, 20, 20, 20, 0);
        SDL_RenderClear(ren);
        drawCircles(ren);
        SDL_RenderPresent(ren);
    }

}

int main(int argc, char *argv[])
{
    cout << "Hello, SDL world!" << endl;

    sdl_init_result sdl = basic_init();
    if (sdl.win == nullptr || sdl.ren == nullptr) {
        return -1;
    }

    main_loop(sdl.ren);

    SDL_DestroyRenderer(sdl.ren);
    SDL_DestroyWindow(sdl.win);
    SDL_Quit();

    return 0;
}
