#include "Particle.h"

#include <algorithm>

#include "../Config.h"

Particle::Particle(const Vec2& position, float mass): radius(0)
{
    this->position = position;
    this->mass = mass;

    if (mass != 0.0f)
    {
        this->inverseMass = 1.0f / mass;
    }
    else
    {
        this->inverseMass = 0.0f;
    }
}

Particle::~Particle()
= default;

void Particle::Integrate(float dt)
{
    acceleration = netForce * inverseMass;

    // Calculate & Clamp velocity
    velocity += acceleration * dt;

    if (velocity.MagnitudeSquared() <= MIN_VELOCITY * MIN_VELOCITY)
    {
        velocity.x = 0.0f;
        velocity.y = 0.0f;
    }

    if (velocity.MagnitudeSquared() >= MAX_VELOCITY * MAX_VELOCITY)
    {
        velocity = velocity.UnitVector() * MAX_VELOCITY;
    }

    position += velocity * dt;

    netForce.x = 0.0f;
    netForce.y = 0.0f;
}

void Particle::AddForce(const Vec2& force)
{
    netForce += force;
}
