#pragma once

#include "../../../math/Vec2.h"

enum ShapeType
{
    CIRCLE,
    POLYGON,
    BOX,
};

struct Shape
{
public:
    virtual ~Shape() = default;

    virtual ShapeType GetType() const = 0;
    virtual float GetMomentOfInertia() const = 0;

    virtual void UpdateVertices(const Vec2& position, float rotation) = 0;

    virtual Shape* Clone() const = 0;
};
