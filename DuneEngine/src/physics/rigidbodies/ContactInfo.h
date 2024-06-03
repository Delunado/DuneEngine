#pragma once
#include "../../math/Vec2.h"

struct Body;

/**
 * - Start and End contact points are from A to B.
 * - Normal direction is from A to B.
 * - Depth is the penetration length.
 */
class ContactInfo
{
public:
    Body* bodyA;
    Body* bodyB;

    Vec2 start;
    Vec2 end;

    Vec2 normal;
    float depth;
};
