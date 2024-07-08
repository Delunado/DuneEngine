#include "World.h"

#include "Constraint.h"
#include "../Config.h"

#include "Force.h"
#include "PenetrationConstraint.h"
#include "../render/DUDraw.h"
#include "rigidbodies/Body.h"
#include "rigidbodies/CollisionDetection.h"
#include "rigidbodies/CollisionResolution.h"
#include "rigidbodies/ContactInfo.h"

World::World(const Vec2& gravity)
{
    _gravity = Vec2(gravity.x, -gravity.y);

    _bodies.reserve(100);
    _constraints.reserve(100);
    _forces.reserve(100);
    _impulses.reserve(100);
    _torques.reserve(100);
}

World::~World()
{
    for (auto body : _bodies)
    {
        delete body;
    }

    for (auto constraint : _constraints)
    {
        delete constraint;
    }
}

void World::AddBody(Body* body)
{
    _bodies.push_back(body);
}

std::vector<Body*>& World::GetBodies()
{
    return _bodies;
}

void World::AddConstraint(Constraint* constraint)
{
    _constraints.push_back(constraint);
}

std::vector<Constraint*>& World::GetConstraints()
{
    return _constraints;
}

void World::AddForce(const Vec2& force)
{
    _forces.push_back(force);
}

void World::AddImpulse(const Vec2& impulse)
{
    _impulses.push_back(impulse);
}

void World::AddTorque(float torque)
{
    _torques.push_back(torque);
}

void World::Update(float dt)
{
    if (_bodies.empty()) return;

    // Temporary, this is for resolving penetration constraints this frame
    _penetrationConstraints.clear();

    for (Body* body : _bodies)
    {
        Vec2 gravityForce = _gravity * body->mass;
        body->AddForce(gravityForce);

        for (Vec2 force : _forces)
        {
            body->AddForce(force);
        }

        for (Vec2 impulse : _impulses)
        {
            body->ApplyLinearImpulse(impulse * body->mass);
        }

        for (float torque : _torques)
        {
            body->AddTorque(torque);
        }
    }

    // First we calculate the forces acting on the bodies
    for (Body* body : _bodies)
    {
        body->IntegrateForces(dt);
    }

    // Then we check for collisions
    for (int i = 0; i <= _bodies.size() - 1; i++)
    {
        for (int j = i + 1; j < _bodies.size(); j++)
        {
            Body* body = _bodies[i];
            Body* otherBody = _bodies[j];

            std::vector<ContactInfo> contacts;
            if (CollisionDetection::IsColliding(body, otherBody, contacts))
            {
                for (ContactInfo& contactInfo : contacts)
                {
                    _penetrationConstraints.emplace_back(contactInfo.bodyA, contactInfo.bodyB, contactInfo.start,
                                                         contactInfo.end, contactInfo.normal);
                }
            }
        }
    }

    float subStepsDetalTime = dt / 6.0f;
    // Pre-solve all constraints, using cached lambda
    for (auto& constraint : _constraints)
    {
        constraint->PreSolve(subStepsDetalTime);
    }

    for (auto& penetration : _penetrationConstraints)
    {
        penetration.PreSolve(subStepsDetalTime);
    }

    // Solve all constraints, will modify velocities
    for (int i = 0; i < 6; i++)
    {
        for (auto& constraint : _constraints)
        {
            constraint->Solve();
        }

        for (auto& penetration : _penetrationConstraints)
        {
            penetration.Solve();
        }
    }

    for (auto& constraint : _constraints)
    {
        constraint->PostSolve();
    }

    for (auto& penetration : _penetrationConstraints)
    {
        penetration.PostSolve();
    }

    // Finally we integrate the velocities to get the new positions
    for (Body* body : _bodies)
    {
        body->IntegrateVelocities(dt);
    }
}

void World::DrawDebug() const
{
    // Draw all penetration constraints info
    for (const PenetrationConstraint& penetration : _penetrationConstraints)
    {
        Vec2 positionA = penetration._bodyA->LocalToWorldSpace(penetration._collisionPointA);
        Vec2 positionB = penetration._bodyB->LocalToWorldSpace(penetration._collisionPointB);
        Vec2 normal = penetration._bodyA->LocalToWorldSpace(penetration._normal).Normal();

        DrawCircle(positionA.x, positionA.y, 2.0f, RED);
        DrawCircle(positionB.x, positionB.y, 2.0f, RED);
        DrawLine(positionA.x, positionA.y, positionA.x + normal.x * 10.0f, positionA.y + normal.y * 10.0f, RED);
    }
}
