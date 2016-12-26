
#include "entity.h"

EntityContainer::EntityContainer()
{

}

EntityContainer::~EntityContainer()
{

}

void EntityContainer::AddEntity(Entity ent)
{
    m_entities.push_back(ent);
}

void EntityContainer::RemoveEntity(Entity ent)
{
    for (size_t i = 0; i < m_entities.size(); ++i) {
        if (m_entities[i].id == ent.id) {
            m_entities.erase(m_entities.begin() + i);
            break;
        }
    }
}

void EntityContainer::RemoveEntity(size_t idx)
{
    m_entities.erase(m_entities.begin() + idx);
}

    
inline size_t EntityContainer::GetComponentIndexForEntity(int entId, ComponentType type)
{

}
