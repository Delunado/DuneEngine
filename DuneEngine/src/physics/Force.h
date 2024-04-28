#pragma once
#include "Particle.h"
#include "../math/Vec2.h"

class Force
{
public:
    static Vec2 GenerateDragForce(const Particle& particle, float dragCoefficient);
    static Vec2 GenerateFrictionForce(const Particle& particle, float frictionCoefficient);
    static Vec2 GenerateGravitationalForce(const Particle& particleA, const Particle& particleB, float gravitationalConstant);
};