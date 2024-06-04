#include "CollisionDetection.h"

#include "ContactInfo.h"
#include "shapes/CircleShape.h"
#include "shapes/Shape.h"

bool CollisionDetection::IsColliding(Body* bodyA, Body* bodyB, ContactInfo& contactInfo)
{
    bool aIsCircle = bodyA->shape->GetType() == CIRCLE;
    bool bIsCircle = bodyB->shape->GetType() == CIRCLE;

    if (aIsCircle && bIsCircle)
    {
        return IsCollidingCircleCircle(bodyA, bodyB, contactInfo);
    }

    return false;
}

bool CollisionDetection::IsCollidingCircleCircle(Body* bodyA, Body* bodyB, ContactInfo& contactInfo)
{
    CircleShape* circleA = dynamic_cast<CircleShape*>(bodyA->shape);
    CircleShape* circleB = dynamic_cast<CircleShape*>(bodyB->shape);

    const Vec2 ab = bodyB->position - bodyA->position;
    const float radiusSum = circleA->radius + circleB->radius;

    bool isColliding = ab.MagnitudeSquared() <= radiusSum * radiusSum;

    if (!isColliding) return false;

    contactInfo.bodyA = bodyA;
    contactInfo.bodyB = bodyB;
    
    contactInfo.normal = ab;
    contactInfo.normal.Normalize();
    
    contactInfo.start = bodyB->position - contactInfo.normal * circleB->radius;
    contactInfo.end = bodyA->position + contactInfo.normal * circleA->radius;
    
    contactInfo.depth = (contactInfo.end - contactInfo.start).Magnitude();

    return true;
}
