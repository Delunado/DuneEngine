#pragma once
#include "../math/VecN.h"

struct Body;

class Constraint
{
public:
    Body* bodyA;
    Body* bodyB;

    //MatMN GetJacobianMatrix();
    VecN GetBiasVector();

    void Solve();
};
