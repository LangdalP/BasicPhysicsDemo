
#include "gfx.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdint.h>

struct colour {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

const static colour col = {247, 64, 119, 255};

int draw_world_polygons(SDL_Renderer *ren, b2World *world)
{
    std::vector<int16_t> vertices_temp_x;
    std::vector<int16_t> vertices_temp_y;
    vertices_temp_x.reserve(4);
    vertices_temp_y.reserve(4);
    for ( b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
        b2Vec2 pos = b->GetPosition();
        // One body can have multiple fixtures
        for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {
            b2Shape::Type shapeType = f->GetType();
            vertices_temp_x.clear();
            vertices_temp_y.clear();
            if ( shapeType == b2Shape::e_circle ) {
                // TODO: Remove assumption that all circles have same center
                b2CircleShape* circleShape = (b2CircleShape*)f->GetShape();
                int16_t radius = (int16_t)( circleShape->m_radius * PX_PER_METER );
                circleRGBA(ren, (int16_t)(pos.x * PX_PER_METER), (int16_t)(pos.y * PX_PER_METER), radius, col.r, col.g, col.b, col.a);
            } else if ( shapeType == b2Shape::e_polygon ) {
                b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
                const int n = polygonShape->GetVertexCount();
                // Naive solution
                for (int i = 0; i < n; ++i) {
                    const b2Vec2 local_pos = polygonShape->GetVertex( i );
                    const b2Vec2 global_pos = b->GetWorldPoint( local_pos );
                    const int16_t x = (int16_t)(global_pos.x * PX_PER_METER);
                    const int16_t y = (int16_t)(global_pos.y * PX_PER_METER);
                    vertices_temp_x.push_back(x);
                    vertices_temp_y.push_back(y);
                }
                polygonRGBA(ren, vertices_temp_x.data(), vertices_temp_y.data(), n, col.r, col.g, col.b, col.a);
            }
        }
    }

}
