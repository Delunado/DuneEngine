#include "CollisionDetection.h"

#include <iostream>

#include "ContactInfo.h"
#include "shapes/CircleShape.h"
#include "shapes/PolygonShape.h"
#include "shapes/Shape.h"

bool CollisionDetection::IsColliding(Body* bodyA, Body* bodyB, ContactInfo& contactInfo)
{
    bool aIsCircle = bodyA->shape->GetType() == CIRCLE;
    bool bIsCircle = bodyB->shape->GetType() == CIRCLE;
    bool aIsPolygon = bodyA->shape->GetType() == POLYGON || bodyA->shape->GetType() == BOX;
    bool bIsPolygon = bodyB->shape->GetType() == POLYGON || bodyB->shape->GetType() == BOX;

    if (aIsCircle && bIsCircle)
    {
        return IsCollidingCircleCircle(bodyA, bodyB, contactInfo);
    }

    if (aIsPolygon && bIsPolygon)
    {
        return IsCollidingPolygonPolygon(bodyA, bodyB, contactInfo);
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

bool CollisionDetection::IsCollidingPolygonPolygon(Body* bodyA, Body* bodyB, ContactInfo& contactInfo)
{
    const PolygonShape* polygonA = dynamic_cast<PolygonShape*>(bodyA->shape);
    const PolygonShape* polygonB = dynamic_cast<PolygonShape*>(bodyB->shape);

    Vec2 edgeA;
    Vec2 edgeB;
    Vec2 startPointA;
    Vec2 startPointB;

    float ABseparation = polygonA->FindMinSeparation(*polygonB, edgeA, startPointA);

    if (ABseparation > 0)
        return false;

    float BAseparation = polygonB->FindMinSeparation(*polygonA, edgeB, startPointB);

    if (BAseparation > 0)
        return false;

    // Contact Info
    contactInfo.bodyA = bodyA;
    contactInfo.bodyB = bodyB;

    if (ABseparation > BAseparation)
    {
        contactInfo.depth = -ABseparation;
        contactInfo.normal = edgeA.NormalPerpendicular();
        contactInfo.start = startPointA;
        contactInfo.end = startPointA + contactInfo.normal * contactInfo.depth;
    }
    else
    {
        contactInfo.depth = -BAseparation;
        contactInfo.normal = -edgeB.NormalPerpendicular();
        contactInfo.start = startPointB - contactInfo.normal * contactInfo.depth;
        contactInfo.end = startPointB;
    }

    return true;
}
