#ifndef L_GFX
#define L_GFX

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#define PX_PER_METER 50

int draw_world_polygons(sf::RenderWindow &ren, b2World *world);

#endif /* ifndef L_GFX */
