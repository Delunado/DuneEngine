#include "Force.h"

Vec2 Force::GenerateDragForce(const Particle& particle, float dragCoefficient)
{
    Vec2 dragForce = Vec2(0, 0);

    const float velocityMagnitudeSquared = particle.velocity.MagnitudeSquared();

    if (velocityMagnitudeSquared > 0)
    {
        const Vec2 dragDirection = particle.velocity.UnitVector() * -1.0f;

        const float dragMagnitude = dragCoefficient * velocityMagnitudeSquared;

        dragForce = dragDirection * dragMagnitude;
    }

    return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Particle& particle, float frictionCoefficient)
{
    const Vec2 frictionDirection = particle.velocity.UnitVector() * -1.0f;

    const float frictionMagnitude = frictionCoefficient * particle.mass;

    Vec2 frictionForce = frictionDirection * frictionMagnitude;

    return frictionForce;
}
