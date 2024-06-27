#pragma once
#include "../math/MatrixMN.h"

struct Body;

class Constraint
{
public:
    virtual ~Constraint() = default;

    Body* _bodyA;
    Body* _bodyB;

    MatrixMN GetInverseMassMatrix();
    VecN GetVelocities() const;

    virtual void PreSolve(const float dt) = 0;
    virtual void Solve() = 0;
    virtual void PostSolve() = 0;
};
