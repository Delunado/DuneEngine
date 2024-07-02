#include "PenetrationConstraint.h"

#include <algorithm>

#include "rigidbodies/Body.h"

PenetrationConstraint::PenetrationConstraint(): Constraint(), _jacobian(2, 6), _cachedLambda(2), _bias(0.0f),
                                                _friction(0.0f)
{
    _bodyA = nullptr;
    _bodyB = nullptr;
    _collisionPointA = Vec2();
    _collisionPointB = Vec2();
    _normal = Vec2();
    _cachedLambda.Zero();
}

PenetrationConstraint::PenetrationConstraint(Body* bodyA, Body* bodyB, const Vec2& collisionPointA,
                                             const Vec2& collisionPointB,
                                             const Vec2& normal): Constraint(), _jacobian(2, 6),
                                                                  _cachedLambda(2), _bias(0.0f), _friction(0.0f)
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
    Vec2 normal = _bodyA->LocalToWorldSpace(_normal).Normal();

    const Vec2 rA = positionA - _bodyA->position;
    const Vec2 rB = positionB - _bodyB->position;

    // Here we calculate the row for penetration
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

    // Here we calculate the row for friction if needed
    _friction = std::max(_bodyA->friction, _bodyB->friction);
    if (_friction > 0.0f)
    {
        Vec2 tangent = normal.Perpendicular();
        const Vec2 jacobianVectorFrictionA = -tangent;
        _jacobian[1][0] = jacobianVectorFrictionA.x; // A Linear velocity X
        _jacobian[1][1] = jacobianVectorFrictionA.y; // A Linear velocity Y

        const float jacobianAngularFrictionA = -rA.Cross(tangent);
        _jacobian[1][2] = jacobianAngularFrictionA; // A Angular velocity

        const Vec2 jacobianVectorFrictionB = tangent;
        _jacobian[1][3] = jacobianVectorFrictionB.x; // B Linear velocity X
        _jacobian[1][4] = jacobianVectorFrictionB.y; // B Linear velocity Y

        const float jacobianAngularFrictionB = rB.Cross(tangent);
        _jacobian[1][5] = jacobianAngularFrictionB; // B Angular velocity
    }

    // Apply the cached lambda
    ApplyImpulses(_cachedLambda);

    // Compute the bias term with bounciness
    Vec2 vA = _bodyA->velocity + Vec2(-_bodyA->angularVelocity * rA.y, _bodyA->angularVelocity * rA.x);
    Vec2 vB = _bodyB->velocity + Vec2(-_bodyB->angularVelocity * rB.y, _bodyB->angularVelocity * rB.x);
    float vRelativeDotNormal = (vB - vA).Dot(normal);

    float elasticity = std::min(_bodyA->restitution, _bodyB->restitution);

    float C = (positionB - positionA).Dot(-normal);
    C = std::max(0.0f, C + 0.01f);
    _bias = (_beta / dt) * C + (elasticity * vRelativeDotNormal);
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

    // Clamp lambda to avoid penetration
    VecN oldLambda = _cachedLambda;
    _cachedLambda += lambda;
    _cachedLambda[0] = std::max(0.0f, _cachedLambda[0]);

    //Clamp friction
    if (_friction > 0.0f)
    {
        const float maxFriction = _cachedLambda[0] * _friction;
        _cachedLambda[1] = std::clamp(_cachedLambda[1], -maxFriction, maxFriction);
    }

    lambda = _cachedLambda - oldLambda;

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
