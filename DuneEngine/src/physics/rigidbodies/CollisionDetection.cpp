#include "CollisionDetection.h"

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

    if (aIsPolygon && bIsCircle)
    {
        return IsCollidingPolygonCircle(bodyA, bodyB, contactInfo);
    }

    if (aIsCircle && bIsPolygon)
    {
        return IsCollidingPolygonCircle(bodyB, bodyA, contactInfo);
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

    const float ABseparation = polygonA->FindMinSeparation(*polygonB, edgeA, startPointA);

    if (ABseparation > 0)
        return false;

    const float BAseparation = polygonB->FindMinSeparation(*polygonA, edgeB, startPointB);

    if (BAseparation > 0)
        return false;

    // Contact Info
    contactInfo.bodyA = bodyA;
    contactInfo.bodyB = bodyB;

    if (ABseparation > BAseparation)
    {
        contactInfo.depth = -ABseparation;
        contactInfo.normal = edgeA.Perpendicular();
        contactInfo.start = startPointA;
        contactInfo.end = startPointA + contactInfo.normal * contactInfo.depth;
    }
    else
    {
        contactInfo.depth = -BAseparation;
        contactInfo.normal = -edgeB.Perpendicular();
        contactInfo.start = startPointB - contactInfo.normal * contactInfo.depth;
        contactInfo.end = startPointB;
    }

    return true;
}

bool CollisionDetection::IsCollidingPolygonCircle(Body* polygon, Body* circle, ContactInfo& contactInfo)
{
    // First we find the nearest edge of the polygon to the circle
    const PolygonShape* polygonShape = dynamic_cast<PolygonShape*>(polygon->shape);
    const CircleShape* circleShape = dynamic_cast<CircleShape*>(circle->shape);
    const std::vector<Vec2>& polygonVertices = polygonShape->worldVertices;

    bool isInside = true;
    Vec2 minCurrentVertex;
    Vec2 minNextVertex;
    float maxProjection = std::numeric_limits<float>::lowest();
    float maxDistanceCircleToEdge = std::numeric_limits<float>::lowest();

    for (int i = 0; i < polygonVertices.size(); i++)
    {
        int currVertex = i;
        int nextVertex = (i + 1) % polygonVertices.size();
        Vec2 edge = polygonShape->EdgeAt(currVertex);
        Vec2 normal = edge.Perpendicular();

        Vec2 vertexToCircleCenter = circle->position - polygonVertices[currVertex];
        float projection = vertexToCircleCenter.Dot(normal);

        if (projection > 0 && projection > maxProjection)
        {
            isInside = false;

            maxDistanceCircleToEdge = projection;
            maxProjection = projection;
            minCurrentVertex = polygonVertices[currVertex];
            minNextVertex = polygonVertices[nextVertex];
        }
        else if (isInside && projection > maxDistanceCircleToEdge)
        {
            maxDistanceCircleToEdge = projection;
            minCurrentVertex = polygonVertices[currVertex];
            minNextVertex = polygonVertices[nextVertex];
        }
    }

    //Now we compute the edges to determine the region of the circle center
    if (!isInside)
    {
        Vec2 edge = minNextVertex - minCurrentVertex;

        // Section A
        Vec2 leftVertexToCircleCenter = circle->position - minCurrentVertex;
        float leftVertexProjection = leftVertexToCircleCenter.Dot(edge);

        if (leftVertexProjection < 0)
        {
            if (leftVertexToCircleCenter.MagnitudeSquared() > circleShape->radius * circleShape->radius)
                // No collision
                return false;

            contactInfo.bodyA = polygon;
            contactInfo.bodyB = circle;
            contactInfo.depth = circleShape->radius - leftVertexToCircleCenter.Magnitude();
            contactInfo.normal = leftVertexToCircleCenter.Normal();
            contactInfo.start = circle->position + (contactInfo.normal * -circleShape->radius);
            contactInfo.end = contactInfo.start + (contactInfo.normal * contactInfo.depth);

            return true;
        }

        // Section B
        Vec2 rightVertexToCircleCenter = circle->position - minNextVertex;
        float rightVertexProjection = rightVertexToCircleCenter.Dot(edge);

        if (rightVertexProjection > 0)
        {
            if (rightVertexToCircleCenter.MagnitudeSquared() > circleShape->radius * circleShape->radius)
                // No collision
                return false;

            contactInfo.bodyA = polygon;
            contactInfo.bodyB = circle;
            contactInfo.depth = circleShape->radius - rightVertexToCircleCenter.Magnitude();
            contactInfo.normal = rightVertexToCircleCenter.Normal();
            contactInfo.start = circle->position + (contactInfo.normal * -circleShape->radius);
            contactInfo.end = contactInfo.start + (contactInfo.normal * contactInfo.depth);

            return true;
        }

        // Section C
        if (maxDistanceCircleToEdge > circleShape->radius)
            // No collision
            return false;

        contactInfo.bodyA = polygon;
        contactInfo.bodyB = circle;
        contactInfo.depth = circleShape->radius - maxDistanceCircleToEdge;
        contactInfo.normal = edge.Perpendicular();
        contactInfo.start = circle->position + (contactInfo.normal * -circleShape->radius);
        contactInfo.end = contactInfo.start + (contactInfo.normal * contactInfo.depth);

        return true;
    }

    // Inside the polygon
    contactInfo.bodyA = polygon;
    contactInfo.bodyB = circle;
    contactInfo.depth = circleShape->radius - maxDistanceCircleToEdge;
    contactInfo.normal = (minNextVertex - minCurrentVertex).Perpendicular();
    contactInfo.start = circle->position + (contactInfo.normal * -circleShape->radius);
    contactInfo.end = contactInfo.start + (contactInfo.normal * contactInfo.depth);

    return true;
}
