#pragma once
#include <vector>

#include "ContactInfo.h"

struct PolygonShape;
struct Body;

struct CollisionDetection
{
public:
    static bool IsColliding(Body* bodyA, Body* bodyB, std::vector<ContactInfo>& contacts);

private:
    static bool IsCollidingCircleCircle(Body* bodyA, Body* bodyB, std::vector<ContactInfo>& contacts);
    static bool IsCollidingPolygonPolygon(Body* bodyA, Body* bodyB, std::vector<ContactInfo>& contacts);
    static bool IsCollidingPolygonCircle(Body* polygon, Body* circle, std::vector<ContactInfo>& contacts);
};
