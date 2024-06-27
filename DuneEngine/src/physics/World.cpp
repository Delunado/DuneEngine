#include "World.h"

#include "Constraint.h"
#include "../Config.h"

#include "Force.h"
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
    for (Body* body : _bodies)
    {
        delete body;
    }

    for (Constraint* constraint : _constraints)
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

void World::Update(float dt) const
{
    if (_bodies.empty()) return;

    for (Body* body : _bodies)
    {
        Vec2 gravityForce = _gravity * body->mass * PIXELS_PER_METER;
        body->AddForce(gravityForce);

        for (Vec2 force : _forces)
        {
            body->AddForce(force);
        }

        for (Vec2 impulse : _impulses)
        {
            body->ApplyLinearImpulse(impulse * body->mass * PIXELS_PER_METER);
        }

        for (float torque : _torques)
        {
            body->AddTorque(torque * PIXELS_PER_METER);
        }
    }

    // First we calculate the forces acting on the bodies
    for (Body* body : _bodies)
    {
        body->IntegrateForces(dt);
    }

    // Pre-solve all constraints, using cached lambda
    for (Constraint* const& constraint : _constraints)
    {
        constraint->PreSolve(dt);
    }

    // Solve all constraints, will modify velocities
    for (int i = 0; i < 5; i++)
    {
        for (Constraint* const& constraint : _constraints)
        {
            constraint->Solve();
        }
    }

    for (Constraint* const& constraint : _constraints)
    {
        constraint->PostSolve();
    }

    // Finally we integrate the velocities to get the new positions
    for (Body* body : _bodies)
    {
        body->IntegrateVelocities(dt);
    }

    CheckCollisions();
}

void World::CheckCollisions() const
{
    for (int i = 0; i <= _bodies.size() - 1; i++)
    {
        for (int j = i + 1; j < _bodies.size(); j++)
        {
            Body* body = _bodies[i];
            Body* otherBody = _bodies[j];

            ContactInfo contactInfo;
            if (CollisionDetection::IsColliding(body, otherBody, contactInfo))
            {
                CollisionResolution::ResolveCollision(contactInfo);
            }
        }
    }
}
