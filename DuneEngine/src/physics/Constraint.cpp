#include "Constraint.h"
#include "rigidbodies/Body.h"

MatrixMN Constraint::GetInverseMassMatrix()
{
    MatrixMN inverseMassMatrix(6, 6);
    inverseMassMatrix.Zero();

    inverseMassMatrix[0][0] = _bodyA->inverseMass;
    inverseMassMatrix[1][1] = _bodyA->inverseMass;
    inverseMassMatrix[2][2] = _bodyA->inverseMomentOfInertia;

    inverseMassMatrix[3][3] = _bodyB->inverseMass;
    inverseMassMatrix[4][4] = _bodyB->inverseMass;
    inverseMassMatrix[5][5] = _bodyB->inverseMomentOfInertia;

    return inverseMassMatrix;
}

VecN Constraint::GetVelocities() const
{
    VecN velocities(6);

    velocities[0] = _bodyA->velocity.x;
    velocities[1] = _bodyA->velocity.y;
    velocities[2] = _bodyA->angularVelocity;
    velocities[3] = _bodyB->velocity.x;
    velocities[4] = _bodyB->velocity.y;
    velocities[5] = _bodyB->angularVelocity;

    return velocities;
}