#include "JointConstraint.h"

#include "rigidbodies/Body.h"

JointConstraint::JointConstraint() : Constraint(), jacobian(1, 6)
{
}

JointConstraint::JointConstraint(Body* bodyA, Body* bodyB, const Vec2& anchorPoint): Constraint(), jacobian(1, 6)
{
    _bodyA = bodyA;
    _bodyB = bodyB;
    anchorA = bodyA->WorldToLocalSpace(anchorPoint);
    anchorB = bodyB->WorldToLocalSpace(anchorPoint);
}

void JointConstraint::Solve()
{
}
