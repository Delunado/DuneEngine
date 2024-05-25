#pragma once

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

    virtual Shape* Clone() const = 0;
};
