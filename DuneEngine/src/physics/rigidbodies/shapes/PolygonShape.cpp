#include "PolygonShape.h"

#include <iostream>
#include <ostream>

PolygonShape::PolygonShape(const std::vector<Vec2>& vertices)
{
    //TODO

    std::cout << "PolygonShape created" << std::endl;
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

PolygonShape* PolygonShape::Clone() const
{
    return new PolygonShape(vertices);
}
