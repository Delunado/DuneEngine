#include "PenetrationConstraint.h"

#include <algorithm>

#include "rigidbodies/Body.h"

PenetrationConstraint::PenetrationConstraint(): Constraint(), _jacobian(1, 6), _cachedLambda(1), _bias(0.0f)
{
    _cachedLambda.Zero();
}

PenetrationConstraint::PenetrationConstraint(Body* bodyA, Body* bodyB, const Vec2& collisionPointA,
                                             const Vec2& collisionPointB,
                                             const Vec2& normal): Constraint(), _jacobian(1, 6),
                                                                  _cachedLambda(1), _bias(0.0f)
{
    _bodyA = bodyA;
    _bodyB = bodyB;
    _collisionPointA = bodyA->WorldToLocalSpace(collisionPointA);
    _collisionPointB = bodyB->WorldToLocalSpace(collisionPointB);
    _normal = bodyA->WorldToLocalSpace(normal);
    _cachedLambda.Zero();
}

void PenetrationConstraint::PreSolve(const float dt)
{
    // Calculate the jacobian
    _jacobian.Zero();

    const Vec2 positionA = _bodyA->LocalToWorldSpace(_collisionPointA);
    const Vec2 positionB = _bodyB->LocalToWorldSpace(_collisionPointB);
    Vec2 normal = _bodyA->LocalToWorldSpace(_normal);

    const Vec2 rA = positionA - _bodyA->position;
    const Vec2 rB = positionB - _bodyB->position;

    const Vec2 jacobianVectorA = -normal;
    _jacobian[0][0] = jacobianVectorA.x; // A Linear velocity X
    _jacobian[0][1] = jacobianVectorA.y; // A Linear velocity Y

    const float jacobianAngularA = -rA.Cross(normal);
    _jacobian[0][2] = jacobianAngularA; // A Angular velocity

    const Vec2 jacobianVectorB = normal;
    _jacobian[0][3] = jacobianVectorB.x; // B Linear velocity X
    _jacobian[0][4] = jacobianVectorB.y; // B Linear velocity Y

    float jacobianAngularB = rB.Cross(normal);
    _jacobian[0][5] = jacobianAngularB; // B Angular velocity

    //ApplyImpulses(_cachedLambda);

    // Compute the bias term
    float C = (positionB - positionA).Dot(-normal);
    C = std::max(0.0f, C + 0.01f);
    _bias = (_beta / dt) * C;
}

void PenetrationConstraint::Solve()
{
    // Then we get the velocities and mass inverse
    const VecN velocities = GetVelocities();
    const MatrixMN inverseMassMatrix = GetInverseMassMatrix();

    // Then we calculate lambda (impulse magnitude)
    const MatrixMN jacobianTranspose = _jacobian.Transpose();

    VecN rightHandSide = _jacobian * velocities * -1.0f;
    rightHandSide[0] -= _bias;

    MatrixMN leftHandSide = _jacobian * inverseMassMatrix * jacobianTranspose;

    VecN lambda = MatrixMN::SolveGaussSeidel(leftHandSide, rightHandSide, 8);
    //_cachedLambda += lambda;

    ApplyImpulses(lambda);
}

void PenetrationConstraint::PostSolve()
{
}

void PenetrationConstraint::ApplyImpulses(const VecN& lambda) const
{
    const MatrixMN jacobianTranspose = _jacobian.Transpose();
    VecN impulses = jacobianTranspose * lambda;

    Vec2 linealImpulseA(impulses[0], impulses[1]);
    float angularImpulseA = impulses[2];

    Vec2 linealImpulseB(impulses[3], impulses[4]);
    float angularImpulseB = impulses[5];

    _bodyA->ApplyLinearImpulse(linealImpulseA);
    _bodyA->ApplyAngularImpulse(angularImpulseA);

    _bodyB->ApplyLinearImpulse(linealImpulseB);
    _bodyB->ApplyAngularImpulse(angularImpulseB);
}