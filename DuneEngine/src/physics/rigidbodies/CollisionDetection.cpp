#include "CollisionDetection.h"

#include "shapes/CircleShape.h"
#include "shapes/Shape.h"

bool CollisionDetection::IsColliding(const Body* bodyA, const Body* bodyB)
{
    bool aIsCircle = bodyA->shape->GetType() == CIRCLE;
    bool bIsCircle = bodyB->shape->GetType() == CIRCLE;

    if (aIsCircle && bIsCircle)
    {
        return IsCollidingCircleCircle(bodyA, bodyB);
    }

    return false;
}

bool CollisionDetection::IsCollidingCircleCircle(const Body* bodyA, const Body* bodyB)
{
    CircleShape* circleA = dynamic_cast<CircleShape*>(bodyA->shape);
    CircleShape* circleB = dynamic_cast<CircleShape*>(bodyB->shape);

    const Vec2 ab = bodyB->position - bodyA->position;
    const float radiusSum = circleA->radius + circleB->radius;

    return ab.MagnitudeSquared() <= radiusSum * radiusSum;
}
