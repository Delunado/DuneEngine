﻿#include "Body.h"

#include <iostream>
#include <ostream>

#include "../../Config.h"

#include "./shapes/Shape.h"

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
    this->elasticity = 1.0f;
    this->friction = 0.5f;
    this->isColliding = false;

    _texture = nullptr;

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

    this->shape->UpdateVertices(position, rotation);
}

Body::~Body()
{
    delete shape;

    std::cout << "Body destroyed" << std::endl;
}

void Body::SetTexture(Texture2D* texture)
{
    _texture = texture;
}

Texture2D* Body::GetTexture() const
{
    return _texture;
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

void Body::ApplyLinearImpulse(const Vec2& impulse)
{
    if (IsStatic())
        return;

    velocity += impulse * inverseMass;
}

void Body::ApplyAngularImpulse(float impulse)
{
    if (IsStatic())
        return;

    angularVelocity += impulse * inverseMomentOfInertia;
}

void Body::ApplyImpulseAtPoint(const Vec2& impulse, const Vec2& point)
{
    if (IsStatic())
        return;

    ApplyLinearImpulse(impulse);
    ApplyAngularImpulse(point.Cross(impulse));
}

void Body::IntegrateForces(float dt)
{
    if (IsStatic())
        return;

    acceleration = netForce * inverseMass;
    velocity += acceleration * dt;

    angularAcceleration = netTorque * inverseMomentOfInertia;
    angularVelocity += angularAcceleration * dt;

    ClearForces();
    ClearTorque();
}

void Body::IntegrateVelocities(float dt)
{
    if (IsStatic())
        return;

    if (velocity.MagnitudeSquared() <= MIN_VELOCITY * MIN_VELOCITY)
    {
        velocity.x = 0.0f;
        velocity.y = 0.0f;
    }

    if (velocity.MagnitudeSquared() >= MAX_VELOCITY * MAX_VELOCITY)
    {
        velocity = velocity.Normal() * MAX_VELOCITY;
    }

    position += velocity * dt;
    rotation += angularVelocity * dt;

    shape->UpdateVertices(position, rotation);
}

Vec2 Body::LocalToWorldSpace(const Vec2& point) const
{
    Vec2 rotatedPoint = point.Rotate(rotation);
    return rotatedPoint + position;
}

Vec2 Body::WorldToLocalSpace(const Vec2& point) const
{
    Vec2 translatedPoint = point - position;
    Vec2 correctPoint = translatedPoint.Rotate(-rotation);

    return correctPoint;
}

bool Body::IsStatic() const
{
    const float epsilon = 0.0001f;
    return fabs(inverseMass - 0.0f) < epsilon;
}
