
#ifndef ENTITY
#define ENTITY 

#include <vector>
#include <cstdint>
#include <Box2D/Box2D.h>

/*
 * Each entity has a 8 bits showing enabled components
 * Component bits:
 *  7   6   5   4   3   2   1   0
 *  -   -   -   -   - ren phy inp
 */
struct Entity {
    // Numeric identifier
    int id;
    // 8 bits showing enabled components
    uint8_t components;
};

enum ComponentType {
    Input = 1,
    Physics = 2,
    Rendering = 4
};

class EntityContainer
{
    std::vector<Entity> m_entities;
    // TODO: Container for input components
    // TODO: Container for physics components
    // TODO: Container for rendering components
    // Mapping from (entity_id, comp_type) to idx in container

    EntityContainer();
    ~EntityContainer();
    
    void AddEntity(Entity ent);
    void RemoveEntity(std::size_t idx);
    void RemoveEntity(Entity ent);

    size_t GetComponentIndexForEntity(int entId, ComponentType type);

};

#endif /* ifndef ENTITY */
