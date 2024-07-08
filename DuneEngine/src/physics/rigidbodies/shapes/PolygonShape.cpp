#include "PolygonShape.h"

#include <iostream>
#include <ostream>

PolygonShape::PolygonShape(const std::vector<Vec2>& vertices): _momentOfInertia(0.0f)
{
    std::cout << "PolygonShape created" << std::endl;

    this->localVertices = vertices;
    this->worldVertices = std::vector(vertices.size(), Vec2(0.0f, 0.0f));
}

PolygonShape::~PolygonShape()
{
    std::cout << "PolygonShape destroyed" << std::endl;
}

ShapeType PolygonShape::GetType() const
{
    return POLYGON;
}

float PolygonShape::GetMomentOfInertia() const
{
    float acc0 = 0.0f;
    float acc1 = 0.0f;

    for (int i = 0; i < localVertices.size(); i++)
    {
        Vec2 a = localVertices[i];
        Vec2 b = localVertices[(i + 1) % localVertices.size()];
        float cross = abs(a.Cross(b));
        acc0 += cross * (a.Dot(a) + a.Dot(b) + b.Dot(b));
        acc1 += cross;
    }

    return acc0 / 6 / acc1;
}

Vec2 PolygonShape::EdgeAt(int index) const
{
    int currentVertex = index;
    int nextVertex = (index + 1) % worldVertices.size();

    Vec2 edge = worldVertices[nextVertex] - worldVertices[currentVertex];
    return edge;
}

void PolygonShape::UpdateVertices(const Vec2& position, float rotation)
{
    for (int i = 0; i < worldVertices.size(); i++)
    {
        worldVertices[i] = localVertices[i].Rotate(rotation);
        worldVertices[i] += position;
    }
}

float PolygonShape::FindMinSeparation(const PolygonShape& other, int& indexReferenceEdge, Vec2& supportPoint) const
{
    float separation = std::numeric_limits<float>::lowest();

    for (int i = 0; i < worldVertices.size(); i++)
    {
        Vec2 vertexA = worldVertices[i];
        Vec2 normal = EdgeAt(i).Perpendicular();

        float minSeparation = std::numeric_limits<float>::max();
        Vec2 minVertex;

        for (Vec2 vertexB : other.worldVertices)
        {
            float projection = (vertexB - vertexA).Dot(normal);

            if (projection < minSeparation)
            {
                minSeparation = projection;
                minVertex = vertexB;
            }
        }

        if (minSeparation > separation)
        {
            indexReferenceEdge = i;
            supportPoint = minVertex;
        }

        separation = std::max(separation, minSeparation);
    }

    return separation;
}

int PolygonShape::FindIncidentEdge(const Vec2& normal) const
{
    int indexIncidentEdge = 0;
    float minProjection = std::numeric_limits<float>::max();

    for (int i = 0; i < this->worldVertices.size(); i++)
    {
        Vec2 edgePerpendicular = EdgeAt(i).Perpendicular();
        float projection = edgePerpendicular.Dot(normal);

        if (projection < minProjection)
        {
            minProjection = projection;
            indexIncidentEdge = i;
        }
    }

    return indexIncidentEdge;
}

int PolygonShape::ClipSegmentToLine(const std::vector<Vec2>& contactsIn, std::vector<Vec2>& contactsOut,
                                    const Vec2& clippingEdge0, const Vec2& clippingEdge1) const
{
    int numberOut = 0;

    Vec2 normal = (clippingEdge1 - clippingEdge0).Normal();
    float distance0 = (contactsIn[0] - clippingEdge0).Cross(normal);
    float distance1 = (contactsIn[1] - clippingEdge0).Cross(normal);

    if (distance0 <= 0.0f)
        contactsOut[numberOut++] = contactsIn[0];

    if (distance1 <= 0.0f)
        contactsOut[numberOut++] = contactsIn[1];

    // If points are in different sides of the plane
    if (distance0 * distance1 < 0)
    {
        float totalDistance = distance0 - distance1;

        // we find the collision point using linear interpolation
        float t = distance0 / totalDistance;
        Vec2 contact = contactsIn[0] + (contactsIn[1] - contactsIn[0]) * t;
        contactsOut[numberOut] = contact;
        numberOut++;
    }

    return numberOut;
}

PolygonShape* PolygonShape::Clone() const
{
    return new PolygonShape(localVertices);
}
