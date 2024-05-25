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
    return 0.0f;
}

BoxShape* BoxShape::Clone() const
{
    return new BoxShape(width, height);
}
