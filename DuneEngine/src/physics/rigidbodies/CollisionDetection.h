#pragma once
#include "Body.h"
#include "ContactInfo.h"

struct PolygonShape;

struct CollisionDetection
{
public:
    static bool IsColliding(Body* bodyA, Body* bodyB, ContactInfo& contactInfo);

private:
    static bool IsCollidingCircleCircle(Body* bodyA, Body* bodyB, ContactInfo& contactInfo);
    static bool IsCollidingPolygonPolygon(Body* bodyA, Body* bodyB, ContactInfo& contactInfo);
    static bool IsCollidingPolygonCircle(Body* polygon, Body* circle, ContactInfo& contactInfo);
};
