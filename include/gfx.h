#ifndef L_GFX
#define L_GFX

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#define PX_PER_METER 50

struct CircleRenderComponent {
    float cx;   // Plan: Update this in physics system
    float cy;
    float angle;
    sf::CircleShape shape;
};

int draw_world_polygons(sf::RenderWindow &ren, b2World *world);
int RunCircleRenderSystem(sf::RenderWindow &ren, std::vector<CircleRenderComponent> comps);

#endif /* ifndef L_GFX */
