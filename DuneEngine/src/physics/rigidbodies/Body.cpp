#include "Body.h"

#include <iostream>
#include <ostream>

#include "../../Config.h"

#include "./shapes/Shape.h"

Body::Body(const Shape& shape, const Vec2& position, float mass)
{
    this->shape = shape.Clone();

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

Body::~Body()
{
    delete shape;
    
    std::cout << "Body destroyed" << std::endl;
}

void Body::Integrate(float dt)
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

void Body::AddForce(const Vec2& force)
{
    netForce += force;
}

void Body::ClearForces()
{
}
