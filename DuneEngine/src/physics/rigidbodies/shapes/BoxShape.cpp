#include "BoxShape.h"

#include <iostream>

BoxShape::BoxShape(const float width, const float height): width(width), height(height)
{
    std::cout << "BoxShape created" << std::endl;
}

BoxShape::~BoxShape()
{
    std::cout << "BoxShape destroyed" << std::endl;
}

ShapeType BoxShape::GetType() const
{
    return BOX;
}

float BoxShape::GetMomentOfInertia() const
{
    return (width * width + height * height) * 0.0833f; // 0.0833f is 1/12
}

BoxShape* BoxShape::Clone() const
{
    return new BoxShape(width, height);
}
