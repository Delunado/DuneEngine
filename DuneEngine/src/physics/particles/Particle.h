#pragma once
#include "../../math/Vec2.h"

struct Particle
{
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;
    Vec2 netForce;

    float radius;

    float mass;
    float inverseMass;

    Particle(const Vec2& position, float mass);
    ~Particle();

    void Integrate(float dt);
    void AddForce(const Vec2& force);
};
