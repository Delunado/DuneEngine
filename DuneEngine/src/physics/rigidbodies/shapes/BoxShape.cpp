#include "BoxShape.h"

#include <iostream>

#include "../../../math/Vec2.h"

BoxShape::BoxShape(const float width, const float height): width(width), height(height)
{
    std::cout << "BoxShape created" << std::endl;

    float horizontalExtent = width * 0.5f;
    float verticalExtent = height * 0.5f;

    Vec2 topLeft(-horizontalExtent, -verticalExtent);
    Vec2 topRight(horizontalExtent, -verticalExtent);
    Vec2 bottomRight(horizontalExtent, verticalExtent);
    Vec2 bottomLeft(-horizontalExtent, verticalExtent);

    // Clockwise order
    localVertices.push_back(topLeft);
    localVertices.push_back(topRight);
    localVertices.push_back(bottomRight);
    localVertices.push_back(bottomLeft);

    worldVertices.push_back(topLeft);
    worldVertices.push_back(topRight);
    worldVertices.push_back(bottomRight);
    worldVertices.push_back(bottomLeft);
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
