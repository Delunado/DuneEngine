#pragma once
#include "Body.h"
#include "ContactInfo.h"

struct CollisionDetection
{
public:
    static bool IsColliding(Body* bodyA, Body* bodyB, ContactInfo& contactInfo);

private:
    static bool IsCollidingCircleCircle(Body* bodyA, Body* bodyB, ContactInfo& contactInfo);
};
