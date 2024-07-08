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

    Vec2 EdgeAt(int index) const;

    void UpdateVertices(const Vec2& position, float rotation) override;
    float FindMinSeparation(const PolygonShape& other, int& indexReferenceEdge, Vec2& supportPoint) const;
    int FindIncidentEdge(const Vec2& normal) const;
    int ClipSegmentToLine(const std::vector<Vec2>& contactsIn, std::vector<Vec2>& contactsOut,
                          const Vec2& clippingEdge0, const Vec2& clippingEdge1) const;

    PolygonShape* Clone() const override;

    std::vector<Vec2> localVertices;
    std::vector<Vec2> worldVertices;

private:
    float _momentOfInertia;
};
