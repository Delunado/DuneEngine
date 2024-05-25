#pragma once
#include "Shape.h"

struct CircleShape : public Shape
{
public:
    CircleShape(const float radius);
    virtual ~CircleShape();

    ShapeType GetType() const override;
    float GetMomentOfInertia() const override;
    CircleShape* Clone() const override;

    float radius;
};
