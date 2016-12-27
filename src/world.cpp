
#include "world.h"

#include <iostream>
#include <fstream>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;


/*
 * Need a way to specify position, type, angle etc
 */

// Input: The json for a single rigidBody
b2ObjectDef load_body_from_json(json j)
{
    b2BodyDef bDef;
    bDef.type = b2_staticBody;

    std::vector<b2PolygonShape> shapes;
    shapes.reserve(30);
    std::vector<b2FixtureDef> fixDefs;
    
    std::vector<b2Vec2> shapeVertices;
    shapeVertices.reserve(4);
    int shapeIdx = 0;
    for (const auto &element : j["polygons"]) {
        b2PolygonShape shape;
        for (const auto &vertex : element) {
            float x = vertex["x"].get<float>() * 2.0;
            float y = vertex["y"].get<float>() * 2.0;
            shapeVertices.push_back(b2Vec2(x, y));
            std::cout << "V: ( " << x << " , " << y << " )" << std::endl;
        }
        std::cout << "Done adding vertices. Added num vertices: " << shapeVertices.size() << std::endl;
        shape.Set(shapeVertices.data(), shapeVertices.size());
        shapes.push_back(shape);
        b2FixtureDef fixDef;
        //fixDef.shape = shapes.data() + shapeIdx;
        fixDef.shape = &shapes[shapes.size()-1];
        fixDef.density = 1;
        fixDef.restitution = 0.1;
        fixDefs.push_back(fixDef);
        shapeVertices.clear();
        ++shapeIdx;
    }
    b2ObjectDef def;
    def.bDef = bDef;
    def.pShapes = shapes;
    def.fDefs = fixDefs;
    def.objName = j["name"].get<std::string>();

    return def;
};

std::vector<b2ObjectDef> load_bodies_from_file(std::string fname)
{
    std::ifstream fStream(fname);
    json j;
    fStream >> j;
    std::vector<b2ObjectDef> objects;

    for (const auto &element : j["rigidBodies"]) {
        objects.push_back(load_body_from_json(element));
        std::cout << "Done with object loading" << std::endl;
    }
    return objects;
}

///////////// Object Store class /////////////////

ObjectStore::ObjectStore(std::vector<b2ObjectDef> objects)
{
    for (const auto &obj : objects) {
        m_objects[obj.objName] = obj;
    }
}

ObjectStore::~ObjectStore()
{

}

b2ObjectDef ObjectStore::GetObject(std::string name) const
{
    return m_objects.at(name);
}

b2World* CreateEmptyWorld()
{
    b2Vec2 grav_vec( 0, -9.81 );
    b2World* world = new b2World( grav_vec );
    return world;
}

b2World* CreateTestWorld()
{
    b2Vec2 grav_vec( 0, -9.81 );
    b2World* world = new b2World( grav_vec );

    // Bodies and fixtures
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    ballBodyDef.position.Set( 0, 5);
    ballBodyDef.angle = 0;
    b2Body *ballBody = world->CreateBody( &ballBodyDef );

    b2CircleShape circleShape;
    circleShape.m_p.Set( 0, 0 );
    circleShape.m_radius = 1;

    b2FixtureDef ballFixtureDef;
    ballFixtureDef.shape = &circleShape;
    ballFixtureDef.density = 1;
    ballFixtureDef.restitution = 1;
    ballBody->CreateFixture( &ballFixtureDef );

    // Edge shapes
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    groundBodyDef.position.Set( 0.0, 0.1 );
    b2Body *groundBody = world->CreateBody( &groundBodyDef );

    b2EdgeShape groundShape;
    groundShape.Set( b2Vec2( -15, 0.0 ), b2Vec2( 15, 0.0 ) );

    b2FixtureDef groundFixtureDef;
    groundFixtureDef.shape = &groundShape;
    groundFixtureDef.density = 1;
    groundBody->CreateFixture( &groundFixtureDef );

    const auto objStore = CreateStoreFromFile("objs.json");
    b2ObjectDef eggDef = objStore.GetObject("Egg");
    eggDef.bDef.position.Set(-2, 3);
    std::cout << "Trying to create egg body" << std::endl;
    auto * body = world->CreateBody( &eggDef.bDef );
    int shapeIdx = 0;
    for (auto &fix : eggDef.fDefs ) {
        std::cout << "Trying to create fixture" << std::endl;
        fix.shape = &eggDef.pShapes[shapeIdx];
        body->CreateFixture( &fix );
        ++shapeIdx;
    }

    b2ObjectDef bananaDef = objStore.GetObject("Banana");
    bananaDef.bDef.position.Set(5, 5);
    std::cout << "Trying to create banana body" << std::endl;
    auto *bananaBody = world->CreateBody( &bananaDef.bDef );
    shapeIdx = 0;
    for (auto &fix : bananaDef.fDefs ) {
        std::cout << "Trying to create fixture" << std::endl;
        fix.shape = &bananaDef.pShapes[shapeIdx];
        bananaBody->CreateFixture( &fix );
        ++shapeIdx;
    }

    return world;
}

ObjectStore CreateStoreFromFile(std::string fname)
{
    const std::vector<b2ObjectDef> bodies = load_bodies_from_file(fname);
    return ObjectStore(bodies);
}
