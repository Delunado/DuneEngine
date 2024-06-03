#pragma once
#include "Body.h"

struct CollisionDetection
{
public:
    static bool IsColliding(const Body* bodyA, const Body* bodyB);

private:
    static bool IsCollidingCircleCircle(const Body* bodyA, const Body* bodyB);
};
