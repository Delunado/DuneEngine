#pragma once

#include <raylib.h>

#include "../../math/Vec2.h"

struct Shape;

struct Body
{
public:
    Body(const Shape& shape, const Vec2& position, float mass);
    ~Body();

    void SetTexture(Texture2D* _texture);
    Texture2D* GetTexture() const;

    void AddForce(const Vec2& force);
    void ClearForces();

    void AddTorque(float torque);
    void ClearTorque();

    void ApplyImpulse(const Vec2& impulse);
    void ApplyImpulseAtPoint(const Vec2& impulse, const Vec2& point);

    void IntegrateForces(float dt);
    void IntegrateVelocities(float dt);

    Vec2 LocalToWorldSpace(const Vec2& point) const;
    Vec2 WorldToLocalSpace(const Vec2& point) const;

    bool IsStatic() const;

    Shape* shape = nullptr;

    // Mass and Inertia
    float mass;
    float inverseMass;
    float momentOfInertia;
    float inverseMomentOfInertia;

    // Linear motion
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    // Angular motion
    float rotation;
    float angularVelocity;
    float angularAcceleration;

    // Forces
    Vec2 netForce;
    float netTorque;

    // Collision
    float restitution;
    float friction;
    bool isColliding;

private:
    //Visuals - Temporal!
    Texture2D* _texture;
};
