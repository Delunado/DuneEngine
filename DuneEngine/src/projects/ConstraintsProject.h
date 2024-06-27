#pragma once
#include "BaseProject.h"

struct Body;
class World;

class ConstraintsProject : public BaseProject
{
public:
    ConstraintsProject();

    void Setup() override;
    void Input() override;
    void Update(float dt) override;
    void FixedUpdate(float dt) override;
    void Render() override;
    void Cleanup() override;

private:
    World* _world;
    Body* _lastJointBody;
};
