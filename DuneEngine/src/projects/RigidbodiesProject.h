#pragma once
#include <vector>

#include "BaseProject.h"
#include "../physics/rigidbodies/ContactInfo.h"

class World;
struct Body;

class RigidbodiesProject : public BaseProject
{
public:
    RigidbodiesProject();

    void Setup() override;
    void Input() override;
    void Update(float dt) override;
    void FixedUpdate(float dt) override;
    void Render() override;
    void Cleanup() override;

private:
    World* _world;
    ContactInfo _contactInfo;

    Body* _character;
};
