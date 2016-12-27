#include "gfx.h"

#include <cstdio>
#include <cstdint>
#include <cmath>
#include "utils.h"
#include "logging.h"

const static sf::Color col(171, 249, 62);
const static sf::Color col_bg(30, 30, 30);
const static float circleOutline = 4.0;
static sf::Vertex tempLine[2];

inline sf::Vector2f RotateVec(sf::Vector2f vec, float angle)
{
    float cs = std::cos(angle);
    float sn = std::sin(angle);

    float new_x = vec.x * cs - vec.y * sn;
    float new_y = vec.x * sn + vec.y * cs;
    return sf::Vector2f(new_x, new_y);
}

// TODO: See if reusing shape(s) gives better performance
inline void DrawCircle(sf::RenderWindow &ren, b2Body *body, b2CircleShape *shape, unsigned sWidth, unsigned sHeight)
{
    float radius = shape->m_radius * PX_PER_METER - circleOutline;
    sf::CircleShape sfShape(radius);
    sfShape.setFillColor(col_bg);
    sfShape.setOutlineColor(col);
    sfShape.setOutlineThickness(circleOutline);
    sfShape.setOrigin(radius, radius);
    sfShape.setPosition(WorldPosToScreenPos(body->GetPosition(), sWidth, sHeight));
    ren.draw(sfShape);
    // Draw a line showing angle
    float angle = body->GetAngle();
    auto circleCenter = sfShape.getPosition();
    auto angleVec = sf::Vector2f(1, 0);
    auto angleVecRotated = RotateVec(angleVec, -angle);
    auto circleEdge = circleCenter + radius * angleVecRotated;
    tempLine[0] = sf::Vertex(circleCenter);
    tempLine[1] = sf::Vertex(circleEdge);
    ren.draw(tempLine, 2, sf::Lines);
}

inline void DrawLine(sf::RenderWindow &ren, b2Body *body, b2EdgeShape *shape, unsigned sWidth, unsigned sHeight)
{
    auto start = WorldPosToScreenPos(body->GetWorldPoint(shape->m_vertex1), sWidth, sHeight);
    auto end = WorldPosToScreenPos(body->GetWorldPoint(shape->m_vertex2), sWidth, sHeight);
    if ( shape->m_hasVertex0 || shape->m_hasVertex3 )
        log_error( "Found ghost vertices that I am ignoring!" );
    tempLine[0] = sf::Vertex(sf::Vector2f(start.x, start.y));
    tempLine[1] = sf::Vertex(sf::Vector2f(end.x, end.y));
    ren.draw(tempLine, 2, sf::Lines);

}

inline void DrawPolygon(sf::RenderWindow &ren, b2Body *body, b2PolygonShape *shape, unsigned sWidth, unsigned sHeight)
{
    sf::ConvexShape cShape;
    int nVertices = shape->GetVertexCount();
    cShape.setPointCount(nVertices);
    for (int i = 0; i < nVertices; ++i) {
        const b2Vec2 worldVertex = body->GetWorldPoint(shape->GetVertex(i));
        const sf::Vector2f screenPos = WorldPosToScreenPos(worldVertex, sWidth, sHeight);
        cShape.setPoint(i, screenPos);
    }
    cShape.setFillColor(col);
    ren.draw(cShape);
}

void DrawWorldPolygons(sf::RenderWindow &ren, b2World *world)
{
    auto screenSize = ren.getSize();
    for ( b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
        // One body can have multiple fixtures
        for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {
            b2Shape::Type shapeType = f->GetType();
            if ( shapeType == b2Shape::e_circle ) {
                b2CircleShape *cShape = (b2CircleShape*)f->GetShape();
                DrawCircle(ren, b, cShape, screenSize.x, screenSize.y);
            }
            else if ( shapeType == b2Shape::e_edge ) {
                b2EdgeShape *edgeShape = (b2EdgeShape*)f->GetShape();
                DrawLine(ren, b, edgeShape, screenSize.x, screenSize.y);
            }
            else if ( shapeType == b2Shape::e_polygon ) {
                b2PolygonShape *polyShape = (b2PolygonShape*)f->GetShape();
                DrawPolygon(ren, b, polyShape, screenSize.x, screenSize.y);
            }
        }
    }
}

