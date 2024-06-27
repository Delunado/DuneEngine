#pragma once
#include "Constraint.h"
#include "../math/Vec2.h"

class JointConstraint : public Constraint
{
public:
    JointConstraint();
    JointConstraint(Body* bodyA, Body* bodyB, const Vec2& anchorPoint);

    void PreSolve(const float dt) override;
    void Solve() override;
    void PostSolve() override;

private:
    void ApplyImpulses(const VecN& lambda) const;

    MatrixMN _jacobian;
    
    VecN _cachedLambda;
    
    float _bias;
    float _beta = 0.15f;

    Vec2 _anchorA; // Local anchor point on body A
    Vec2 _anchorB; // Local anchor point on body B
};
