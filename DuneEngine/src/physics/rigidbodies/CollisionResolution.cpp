#include "CollisionResolution.h"

#include <algorithm>

#include "Body.h"

void CollisionResolution::ResolvePenetration(ContactInfo& contactInfo)
{
    if (contactInfo.bodyA->IsStatic() && contactInfo.bodyB->IsStatic()) return;

    float displacement = contactInfo.depth / (contactInfo.bodyA->inverseMass + contactInfo.bodyB->inverseMass);
    float displacementA = displacement * contactInfo.bodyA->inverseMass;
    float displacementB = displacement * contactInfo.bodyB->inverseMass;

    contactInfo.bodyA->position -= contactInfo.normal * displacementA;
    contactInfo.bodyB->position += contactInfo.normal * displacementB;
}

void CollisionResolution::ResolveCollision(ContactInfo& contactInfo)
{
    ResolvePenetration(contactInfo);

    Body* bodyA = contactInfo.bodyA;
    Body* bodyB = contactInfo.bodyB;

    float e = std::min(bodyA->restitution, bodyB->restitution);

    float relativeVelocityAlongNormal = (bodyA->velocity - bodyB->velocity).Dot(contactInfo.normal);

    float impulseMagnitude = -(1 + e) * relativeVelocityAlongNormal / (bodyA->inverseMass + bodyB->inverseMass);
    Vec2 impulse = contactInfo.normal * impulseMagnitude;

    bodyA->ApplyImpulse(impulse);
    bodyB->ApplyImpulse(-impulse);
}
