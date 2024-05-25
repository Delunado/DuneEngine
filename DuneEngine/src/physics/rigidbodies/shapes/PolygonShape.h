#pragma once
#include <vector>

#include "Shape.h"
#include "../../../math/Vec2.h"

struct PolygonShape : public Shape
{
public:
    PolygonShape() = default;
    PolygonShape(const std::vector<Vec2>& vertices);
    virtual ~PolygonShape();

    ShapeType GetType() const override;
    float GetMomentOfInertia() const override;
    PolygonShape* Clone() const override;

    std::vector<Vec2> vertices;
};
