#ifndef L_UTILS
#define L_UTILS

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "gfx.h"

inline sf::Vector2f WorldPosToScreenPos(b2Vec2 worldPos, unsigned int sWidth, unsigned int sHeight)
{
    float x = worldPos.x * PX_PER_METER + sWidth / 2;
    float y = sHeight - worldPos.y * PX_PER_METER;
    return sf::Vector2f(x, y);
}

#endif /* ifndef L_UTILS */

