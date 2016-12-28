
#ifndef L_WORLD
#define L_WORLD 

#include <vector>
#include <unordered_map>
#include <string>
#include <Box2D/Box2D.h>

struct b2ObjectDef {
    b2BodyDef bDef;
    std::vector<b2PolygonShape> pShapes;
    std::vector<b2FixtureDef> fDefs;
    std::string objName;
    std::string spriteName;
};

class ObjectStore
{
public:
    std::unordered_map<std::string, b2ObjectDef> m_objects;

    ObjectStore(std::vector<b2ObjectDef> objects);
    ~ObjectStore();

    b2ObjectDef GetObject(std::string name) const;
    b2ObjectDef GetObjectAsDynamic(std::string name) const;
    b2ObjectDef GetObjectAtPos(std::string name, float x, float y) const;
    b2ObjectDef GetObjectAsDynamicAtPos(std::string name, float x, float y) const;
};

b2World* CreateEmptyWorld();
b2World* CreateTestWorld();
ObjectStore CreateStoreFromFile(std::string fname);

b2Body* CreateBodyFromObject(b2ObjectDef obj, b2World* world);


#endif /* ifndef L_WORLD */
