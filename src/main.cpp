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
#include "world.h"
#include "gfx.h"

using std::cout;
using std::endl;

const static int VELOCITY_ITERATIONS = 8;
const static int POSITION_ITERATIONS = 3;

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

void main_loop(SDL_Renderer *ren, b2World *world)
{
    SDL_Event e;
    uint32_t t = 0;
    uint32_t dt = 8;   // Aiming for 100 fps
    uint32_t current_time = SDL_GetTicks();
    uint32_t loop_start_time = current_time;
    uint32_t frame_counter = 0;

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
            world->Step((float)delta_time/1000.0, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
            frame_time -= delta_time;
            t += delta_time;
        }

        // Rendering
        SDL_SetRenderDrawColor(ren, 20, 20, 20, 0);
        SDL_RenderClear(ren);
        draw_world_polygons(ren, world);
        SDL_RenderPresent(ren);
        ++frame_counter;
    }

    uint32_t loop_duration = current_time - loop_start_time;
    float avg_fps = (float)frame_counter/(loop_duration/1000.0);

    printf("Avg. fps: %f\n", avg_fps);

}

int main(int argc, char *argv[])
{
    cout << "Hello, SDL world!" << endl;

    sdl_init_result sdl = basic_init();
    if (sdl.win == nullptr || sdl.ren == nullptr) {
        return -1;
    }

    // World and bodies are never cleaned up... should they?
    b2World *world = create_world_with_test_objects();

    main_loop(sdl.ren, world);

    SDL_DestroyRenderer(sdl.ren);
    SDL_DestroyWindow(sdl.win);
    SDL_Quit();

    return 0;
}
