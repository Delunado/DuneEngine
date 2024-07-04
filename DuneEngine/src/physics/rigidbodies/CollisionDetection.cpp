#include "CollisionDetection.h"
#include "Body.h"
#include "ContactInfo.h"
#include "shapes/CircleShape.h"
#include "shapes/PolygonShape.h"
#include "shapes/Shape.h"

bool CollisionDetection::IsColliding(Body* bodyA, Body* bodyB, std::vector<ContactInfo>& contacts)
{
    bool aIsCircle = bodyA->shape->GetType() == CIRCLE;
    bool bIsCircle = bodyB->shape->GetType() == CIRCLE;
    bool aIsPolygon = bodyA->shape->GetType() == POLYGON || bodyA->shape->GetType() == BOX;
    bool bIsPolygon = bodyB->shape->GetType() == POLYGON || bodyB->shape->GetType() == BOX;

    if (aIsCircle && bIsCircle)
    {
        return IsCollidingCircleCircle(bodyA, bodyB, contacts);
    }

    if (aIsPolygon && bIsPolygon)
    {
        return IsCollidingPolygonPolygon(bodyA, bodyB, contacts);
    }

    if (aIsPolygon && bIsCircle)
    {
        return IsCollidingPolygonCircle(bodyA, bodyB, contacts);
    }

    if (aIsCircle && bIsPolygon)
    {
        return IsCollidingPolygonCircle(bodyB, bodyA, contacts);
    }

    return false;
}

bool CollisionDetection::IsCollidingCircleCircle(Body* bodyA, Body* bodyB, std::vector<ContactInfo>& contacts)
{
    CircleShape* circleA = dynamic_cast<CircleShape*>(bodyA->shape);
    CircleShape* circleB = dynamic_cast<CircleShape*>(bodyB->shape);

    const Vec2 ab = bodyB->position - bodyA->position;
    const float radiusSum = circleA->radius + circleB->radius;

    bool isColliding = ab.MagnitudeSquared() <= radiusSum * radiusSum;

    if (!isColliding) return false;

    ContactInfo contactInfo;

    contactInfo.bodyA = bodyA;
    contactInfo.bodyB = bodyB;

    contactInfo.normal = ab;
    contactInfo.normal.Normalize();

    contactInfo.start = bodyB->position - contactInfo.normal * circleB->radius;
    contactInfo.end = bodyA->position + contactInfo.normal * circleA->radius;

    contactInfo.depth = (contactInfo.end - contactInfo.start).Magnitude();

    contacts.push_back(contactInfo);

    return true;
}

bool CollisionDetection::IsCollidingPolygonPolygon(Body* bodyA, Body* bodyB, std::vector<ContactInfo>& contacts)
{
    PolygonShape* polygonA = dynamic_cast<PolygonShape*>(bodyA->shape);
    PolygonShape* polygonB = dynamic_cast<PolygonShape*>(bodyB->shape);

    int indexReferenceEdgeA;
    Vec2 supportPointA;

    int indexReferenceEdgeB;
    Vec2 supportPointB;

    const float ABseparation = polygonA->FindMinSeparation(*polygonB, indexReferenceEdgeA, supportPointA);

    if (ABseparation > 0)
        return false;

    const float BAseparation = polygonB->FindMinSeparation(*polygonA, indexReferenceEdgeB, supportPointB);

    if (BAseparation > 0)
        return false;

    // Setting the reference and incident based on the bigger separation
    PolygonShape* referencePolygon;
    PolygonShape* incidentPolygon;
    int indexReferenceEdge;

    if (ABseparation > BAseparation)
    {
        referencePolygon = polygonA;
        incidentPolygon = polygonB;
        indexReferenceEdge = indexReferenceEdgeA;
    }
    else
    {
        referencePolygon = polygonB;
        incidentPolygon = polygonA;
        indexReferenceEdge = indexReferenceEdgeB;
    }

    // Finding the reference edge
    Vec2 referenceEdge = referencePolygon->EdgeAt(indexReferenceEdge);

    // Doing clipping
    int incidentIndex = incidentPolygon->FindIncidentEdge(referenceEdge.Perpendicular());
    int incidentIndexNext = (incidentIndex + 1) % incidentPolygon->worldVertices.size();
    Vec2 vertexIncident = incidentPolygon->worldVertices[incidentIndex];
    Vec2 vertexIncidentNext = incidentPolygon->worldVertices[incidentIndexNext];

    std::vector<Vec2> contactPoints = {vertexIncident, vertexIncidentNext};
    std::vector<Vec2> clippedPoints = contactPoints;

    for (int i = 0; i < referencePolygon->worldVertices.size(); i++)
    {
        if (i == indexReferenceEdge)
            continue;

        Vec2 clippingEdge0 = referencePolygon->worldVertices[i];
        Vec2 clippingEdge1 = referencePolygon->worldVertices[(i + 1) % referencePolygon->worldVertices.size()];
        int numberOfClippedPoints = referencePolygon->
            ClipSegmentToLine(contactPoints, clippedPoints, clippingEdge0, clippingEdge1);

        if (numberOfClippedPoints < 2)
            break;

        contactPoints = clippedPoints;
    }

    // Loop all clipped pints, only considering the ones with negative separation (penetrating ones)
    Vec2 vertexReference = referencePolygon->worldVertices[indexReferenceEdge];

    for (Vec2 vertexClip : clippedPoints)
    {
        float separation = (vertexClip - vertexReference).Dot(referenceEdge.Perpendicular());
        if (separation <= 0)
        {
            ContactInfo contactInfo;
            contactInfo.bodyA = bodyA;
            contactInfo.bodyB = bodyB;
            contactInfo.normal = referenceEdge.Perpendicular();
            contactInfo.start = vertexClip;
            contactInfo.end = vertexClip + contactInfo.normal * -separation;

            if (BAseparation >= ABseparation)
            {
                std::swap(contactInfo.start, contactInfo.end); //Start points are always from A to B
                contactInfo.normal *= -1.0f;
            }

            contacts.push_back(contactInfo);
        }
    }

    return true;
}

bool CollisionDetection::IsCollidingPolygonCircle(Body* polygon, Body* circle, std::vector<ContactInfo>& contacts)
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

    ContactInfo contactInfo;

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

            contacts.push_back(contactInfo);

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

            contacts.push_back(contactInfo);

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

        contacts.push_back(contactInfo);

        return true;
    }

    // Inside the polygon
    contactInfo.bodyA = polygon;
    contactInfo.bodyB = circle;
    contactInfo.depth = circleShape->radius - maxDistanceCircleToEdge;
    contactInfo.normal = (minNextVertex - minCurrentVertex).Perpendicular();
    contactInfo.start = circle->position + (contactInfo.normal * -circleShape->radius);
    contactInfo.end = contactInfo.start + (contactInfo.normal * contactInfo.depth);

    contacts.push_back(contactInfo);

    return true;
}
