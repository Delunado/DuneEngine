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
    return 0.0f;
}

void PolygonShape::UpdateVertices(const Vec2& position, float rotation)
{
    for (int i = 0; i < worldVertices.size(); i++)
    {
        worldVertices[i] = localVertices[i].Rotate(rotation);
        worldVertices[i] += position;
    }
}

PolygonShape* PolygonShape::Clone() const
{
    return new PolygonShape(localVertices);
}
