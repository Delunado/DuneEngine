#include "World.h"

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
}

void World::AddBody(Body* body)
{
    _bodies.push_back(body);
}

std::vector<Body*>& World::GetBodies()
{
    return _bodies;
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
    for (Body* body : _bodies)
    {
        Vec2 gravityForce = _gravity * body->mass * PIXELS_PER_METER;
        body->AddForce(gravityForce);

        for (Vec2 force : _forces)
        {
            body->AddForce(force * PIXELS_PER_METER);
        }

        for (Vec2 impulse : _impulses)
        {
            body->ApplyImpulse(impulse * body->mass * PIXELS_PER_METER);
        }

        for (float torque : _torques)
        {
            body->AddTorque(torque * PIXELS_PER_METER);
        }

        body->Update(dt);
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
