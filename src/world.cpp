
#include "world.h"

b2World* create_world()
{
    b2Vec2 grav_vec( 0, -9.81 );
    b2World* world = new b2World( grav_vec );
    return world;
}

b2World* create_world_with_test_objects()
{
    b2Vec2 grav_vec( 0, -9.81 );
    b2World* world = new b2World( grav_vec );

    // Bodies and fixtures
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    ballBodyDef.position.Set( 10, 10 );
    ballBodyDef.angle = 0;
    b2Body *ballBody = world->CreateBody( &ballBodyDef );

    b2CircleShape circleShape;
    circleShape.m_p.Set(0, 0);
    circleShape.m_radius = 1;

    b2FixtureDef ballFixtureDef;
    ballFixtureDef.shape = &circleShape;
    ballFixtureDef.density = 1;
    ballFixtureDef.restitution = 1;
    ballBody->CreateFixture( &ballFixtureDef );

    // Edge shapes
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    groundBodyDef.position.Set( 0, 0 );
    b2Body *groundBody = world->CreateBody( &groundBodyDef );

    b2EdgeShape groundShape;
    groundShape.Set( b2Vec2( -15, 0 ), b2Vec2( 15, 0 ) );

    b2FixtureDef groundFixtureDef;
    groundFixtureDef.shape = &groundShape;
    groundFixtureDef.density = 1;
    groundBody->CreateFixture( &groundFixtureDef );

    return world;
}
