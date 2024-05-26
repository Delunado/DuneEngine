#pragma once

#include "../../math/Vec2.h"

struct Shape;

struct Body
{
public:
    Body(const Shape& shape, const Vec2& position, float mass);
    ~Body();

    void AddForce(const Vec2& force);
    void ClearForces();

    void AddTorque(float torque);
    void ClearTorque();

    void IntegrateLinear(float dt);
    void IntegrateAngular(float dt);

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
};
