#include "Force.h"

#include <algorithm>
#include "../Config.h"
#include "rigidbodies/Body.h"

Vec2 Force::GenerateDragForce(const Particle& particle, float dragCoefficient)
{
    Vec2 dragForce = Vec2(0, 0);

    const float velocityMagnitudeSquared = particle.velocity.MagnitudeSquared();

    if (velocityMagnitudeSquared > 0)
    {
        const Vec2 dragDirection = particle.velocity.Normal() * -1.0f;

        const float dragMagnitude = dragCoefficient * velocityMagnitudeSquared;

        dragForce = dragDirection * dragMagnitude;
    }

    return dragForce;
}

Vec2 Force::GenerateDragForce(const Body& body, float dragCoefficient)
{
    Vec2 dragForce = Vec2(0, 0);

    const float velocityMagnitudeSquared = body.velocity.MagnitudeSquared();

    if (velocityMagnitudeSquared > 0)
    {
        const Vec2 dragDirection = body.velocity.Normal() * -1.0f;

        const float dragMagnitude = dragCoefficient * velocityMagnitudeSquared;

        dragForce = dragDirection * dragMagnitude;
    }

    return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Particle& particle, float frictionCoefficient)
{
    const Vec2 frictionDirection = particle.velocity.Normal() * -1.0f;

    const float frictionMagnitude = frictionCoefficient * particle.mass;

    Vec2 frictionForce = frictionDirection * frictionMagnitude;

    return frictionForce;
}

Vec2 Force::GenerateGravitationalForce(const Particle& particleA, const Particle& particleB,
                                       float gravitationalConstant)
{
    const Vec2 direction = particleB.position - particleA.position;
    const Vec2 attractionDirection = direction.Normal();

    float distanceSquared = direction.MagnitudeSquared();
    // We clamp the distance to make it more interesting
    distanceSquared = std::clamp(distanceSquared, MIN_GRAVITATIONAL_DISTANCE, MAX_GRAVITATIONAL_DISTANCE);

    const float attractionMagnitude = gravitationalConstant * (particleA.mass * particleB.mass) / distanceSquared;

    Vec2 attractionForce = attractionDirection * attractionMagnitude;

    return attractionForce;
}

Vec2 Force::GenerateSpringForce(const Particle& particle, Vec2 anchor, float restLength, float springConstant)
{
    const Vec2 direction = particle.position - anchor;

    const float displacement = direction.Magnitude() - restLength;

    const Vec2 springDirection = direction.Normal();
    const float springMagnitude = -springConstant * displacement;

    Vec2 springForce = springDirection * springMagnitude;

    return springForce;
}
