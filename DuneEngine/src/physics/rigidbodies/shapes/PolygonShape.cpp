#include "PolygonShape.h"

#include <iostream>
#include <ostream>

PolygonShape::PolygonShape(const std::vector<Vec2>& vertices)
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
    return 4000.0f;
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

float PolygonShape:: FindMinSeparation(const PolygonShape& other, Vec2& axis, Vec2& point) const
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
            axis = this->EdgeAt(i);
            point = minVertex;
        }

        separation = std::max(separation, minSeparation);
    }

    return separation;
}

PolygonShape* PolygonShape::Clone() const
{
    return new PolygonShape(localVertices);
}
