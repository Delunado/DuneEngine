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

CircleShape* CircleShape::Clone() const
{
    return new CircleShape(radius);
}
