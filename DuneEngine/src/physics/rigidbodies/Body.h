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

    void Integrate(float dt);

    Shape* shape = nullptr;

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    Vec2 netForce;

    float mass;
    float inverseMass;
};
