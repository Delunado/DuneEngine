#pragma once
#include "PolygonShape.h"
#include "Shape.h"

struct BoxShape : public PolygonShape
{
public:
    BoxShape(const float width, const float height);
    virtual ~BoxShape();

    ShapeType GetType() const override;
    float GetMomentOfInertia() const override;
    BoxShape* Clone() const override;

    float width;
    float height;
};
