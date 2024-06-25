#pragma once
#include "Constraint.h"
#include "../math/Vec2.h"

class JointConstraint : public Constraint
{
public:
    JointConstraint();
    JointConstraint(Body* bodyA, Body* bodyB, const Vec2& anchorPoint);

    void Solve() override;

private:
    MatrixMN jacobian;

    Vec2 anchorA; // Local anchor point on body A
    Vec2 anchorB; // Local anchor point on body B
};
