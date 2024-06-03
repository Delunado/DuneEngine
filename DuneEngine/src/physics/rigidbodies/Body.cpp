#include "Body.h"

#include <iostream>
#include <ostream>

#include "../../Config.h"

#include "./shapes/Shape.h"
#include "shapes/PolygonShape.h"

Body::Body(const Shape& shape, const Vec2& position, float mass)
{
    this->shape = shape.Clone();

    this->position = position;
    this->mass = mass;
    this->velocity = Vec2(0.0f, 0.0f);
    this->acceleration = Vec2(0.0f, 0.0f);
    this->rotation = 0.0f;
    this->angularVelocity = 0.0f;
    this->angularAcceleration = 0.0f;
    this->netForce = Vec2(0.0f, 0.0f);
    this->netTorque = 0.0f;

    if (mass != 0.0f)
    {
        this->inverseMass = 1.0f / mass;
    }
    else
    {
        this->inverseMass = 0.0f;
    }

    this->momentOfInertia = this->shape->GetMomentOfInertia() * mass;

    if (momentOfInertia != 0.0f)
    {
        this->inverseMomentOfInertia = 1.0f / momentOfInertia;
    }
    else
    {
        this->inverseMomentOfInertia = 0.0f;
    }
}

Body::~Body()
{
    delete shape;

    std::cout << "Body destroyed" << std::endl;
}

void Body::IntegrateLinear(float dt)
{
    acceleration = netForce * inverseMass;

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

    ClearForces();
}

void Body::IntegrateAngular(float dt)
{
    angularAcceleration = netTorque * inverseMomentOfInertia;

    angularVelocity += angularAcceleration * dt;

    rotation += angularVelocity * dt;

    ClearTorque();
}

void Body::AddForce(const Vec2& force)
{
    netForce += force;
}

void Body::ClearForces()
{
    netForce.x = 0.0f;
    netForce.y = 0.0f;
}

void Body::AddTorque(float torque)
{
    netTorque += torque;
}

void Body::ClearTorque()
{
    netTorque = 0.0f;
}

void Body::Update(const float dt)
{
    IntegrateLinear(dt);
    IntegrateAngular(dt);

    if (shape->GetType() == BOX || shape->GetType() == POLYGON)
    {
        PolygonShape* polygon = dynamic_cast<PolygonShape*>(shape);
        polygon->UpdateVertices(position, rotation);
    }
}
