#include "CollisionResolution.h"

#include <algorithm>
#include <iostream>

#include "Body.h"
#include "shapes/Shape.h"

void CollisionResolution::ResolvePenetration(ContactInfo& contactInfo)
{
    if (contactInfo.bodyA->IsStatic() && contactInfo.bodyB->IsStatic()) return;

    float displacement = contactInfo.depth / (contactInfo.bodyA->inverseMass + contactInfo.bodyB->inverseMass);
    float displacementA = displacement * contactInfo.bodyA->inverseMass;
    float displacementB = displacement * contactInfo.bodyB->inverseMass;

    contactInfo.bodyA->position -= contactInfo.normal * displacementA;
    contactInfo.bodyB->position += contactInfo.normal * displacementB;

    contactInfo.bodyA->shape->UpdateVertices(contactInfo.bodyA->position, contactInfo.bodyA->rotation);
    contactInfo.bodyB->shape->UpdateVertices(contactInfo.bodyB->position, contactInfo.bodyB->rotation);
}

void CollisionResolution::ResolveCollision(ContactInfo& contactInfo)
{
    ResolvePenetration(contactInfo);

    Body* bodyA = contactInfo.bodyA;
    Body* bodyB = contactInfo.bodyB;

    float elasticCoefficient = std::min(bodyA->restitution, bodyB->restitution);
    float frictionCoefficient = std::min(bodyA->friction, bodyB->friction);

    //Here we calculate the relative velocity of the two bodies at the point of contact,
    //taking into account the angular velocity of the bodies
    Vec2 rA = contactInfo.end - bodyA->position;
    Vec2 rB = contactInfo.start - bodyB->position;
    Vec2 velocityA = bodyA->velocity + (Vec2(-bodyA->angularVelocity * rA.y, bodyA->angularVelocity * rA.x));
    Vec2 velocityB = bodyB->velocity + (Vec2(-bodyB->angularVelocity * rB.y, bodyB->angularVelocity * rB.x));
    const Vec2 relativeVelocity = velocityA - velocityB;

    // Then, we calculate the impulse magnitude for the normal direction
    float relativeVelocityNormal = relativeVelocity.Dot(contactInfo.normal);
    float impulseMagnitudeNormal = -(1 + elasticCoefficient) * relativeVelocityNormal /
    ((bodyA->inverseMass + bodyB->inverseMass) +
        rA.Cross(contactInfo.normal) * rA.Cross(contactInfo.normal) * bodyA->inverseMomentOfInertia +
        rB.Cross(contactInfo.normal) * rB.Cross(contactInfo.normal) * bodyB->inverseMomentOfInertia);
    Vec2 impulseNormal = contactInfo.normal * impulseMagnitudeNormal;

    // Finally, we calculate the impulse magnitude for the tangent direction
    Vec2 tangent = contactInfo.normal.Perpendicular();
    float relativeVelocityTangent = relativeVelocity.Dot(tangent);
    float impulseMagnitudeTangent = frictionCoefficient * -(1 + elasticCoefficient) * relativeVelocityTangent /
    ((bodyA->inverseMass + bodyB->inverseMass) +
        rA.Cross(tangent) * rA.Cross(tangent) * bodyA->inverseMomentOfInertia +
        rB.Cross(tangent) * rB.Cross(tangent) * bodyB->inverseMomentOfInertia);
    Vec2 impulseTangent = tangent * impulseMagnitudeTangent;

    Vec2 finalImpulse = impulseNormal + impulseTangent;

    bodyA->ApplyImpulseAtPoint(finalImpulse, rA);
    bodyB->ApplyImpulseAtPoint(-finalImpulse, rB);
}
