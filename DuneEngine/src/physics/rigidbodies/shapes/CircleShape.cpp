#include "CircleShape.h"

#include <iostream>

CircleShape::CircleShape(const float radius) : radius(radius)
{
    std::cout << "CircleShape created" << std::endl;
}

CircleShape::~CircleShape()
{
    std::cout << "CircleShape destroyed" << std::endl;
}

ShapeType CircleShape::GetType() const
{
    return CIRCLE;
}

float CircleShape::GetMomentOfInertia() const
{
    return radius * radius * 0.5f;
}

void CircleShape::UpdateVertices(const Vec2& position, float rotation)
{
    return; // Do nothing
}

CircleShape* CircleShape::Clone() const
{
    return new CircleShape(radius);
}
