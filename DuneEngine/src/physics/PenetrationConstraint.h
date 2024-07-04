#pragma once
#include "Constraint.h"
#include "../math/Vec2.h"

class PenetrationConstraint : public Constraint
{
public:
    PenetrationConstraint();
    PenetrationConstraint(Body* bodyA, Body* bodyB, const Vec2& collisionPointA, const Vec2& collisionPointB,
                          const Vec2& normal);

    void PreSolve(const float dt) override;
    void Solve() override;
    void PostSolve() override;

private:
    void ApplyImpulses(const VecN& lambda) const;

    MatrixMN _jacobian;
    VecN _cachedLambda;

    float _bias;
    float _beta = 0.8f;

    Vec2 _collisionPointA; // Local collision point on body A
    Vec2 _collisionPointB; // Local collision point on body B
    Vec2 _normal;
    float _friction;
};
