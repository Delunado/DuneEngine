#include "CollisionResolution.h"

#include "Body.h"

void CollisionResolution::ResolvePenetration(ContactInfo& contactInfo)
{
    float displacement = contactInfo.depth / (contactInfo.bodyA->inverseMass + contactInfo.bodyB->inverseMass);
    float displacementA = displacement * contactInfo.bodyA->inverseMass;
    float displacementB = displacement * contactInfo.bodyB->inverseMass;

    contactInfo.bodyA->position -= contactInfo.normal * displacementA;
    contactInfo.bodyB->position += contactInfo.normal * displacementB;
}
