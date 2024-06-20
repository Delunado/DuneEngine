#pragma once
#include <vector>

#include "BaseProject.h"
#include "../math/Vec2.h"

struct Particle;

class DragonProject : public BaseProject
{
public:
    DragonProject();

    void Setup() override;
    void Input() override;
    void Update(float dt) override;
    void FixedUpdate(float dt) override;
    void Render() override;
    void Cleanup() override;

private:
    Vec2 _currentMousePosition;
    std::vector<Particle*> _particles;

    Vec2 _keyboardForce;

    float _springConstant;
    float _restLength;
};
