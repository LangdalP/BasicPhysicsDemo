#ifndef L_GFX
#define L_GFX

#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>

#define PX_PER_METER 50

int draw_world_polygons(SDL_Renderer *ren, b2World *world);

#endif /* ifndef L_GFX */
