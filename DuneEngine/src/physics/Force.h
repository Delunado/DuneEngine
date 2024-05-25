#pragma once

#include "particles/Particle.h"
#include "rigidbodies/Body.h"
#include "../math/Vec2.h"

class Force
{
public:
    static Vec2 GenerateDragForce(const Particle& particle, float dragCoefficient);
    static Vec2 GenerateDragForce(const Body& body, float dragCoefficient);

    static Vec2 GenerateFrictionForce(const Particle& particle, float frictionCoefficient);
    static Vec2 GenerateGravitationalForce(const Particle& particleA, const Particle& particleB,
                                           float gravitationalConstant);
    static Vec2 GenerateSpringForce(const Particle& particle, Vec2 anchor, float restLength, float springConstant);
};
