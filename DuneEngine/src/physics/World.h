#pragma once
#include <vector>

#include "PenetrationConstraint.h"
#include "../math/Vec2.h"

class Constraint;
struct Body;

class World
{
public:
    World(const Vec2& gravity);
    ~World();

    void AddBody(Body* body);
    std::vector<Body*>& GetBodies();

    void AddConstraint(Constraint* constraint);
    std::vector<Constraint*>& GetConstraints();

    void AddForce(const Vec2& force);
    void AddImpulse(const Vec2& impulse);
    void AddTorque(float torque);

    void Update(float dt);
    void DrawDebug() const;

private:
    void CheckCollisions() const;

    Vec2 _gravity;

    std::vector<Body*> _bodies;
    std::vector<Constraint*> _constraints;

    std::vector<Vec2> _forces;
    std::vector<Vec2> _impulses;
    std::vector<float> _torques;

    std::vector<PenetrationConstraint> _penetrationConstraints;
};
