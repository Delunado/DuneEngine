#pragma once
#include <vector>

#include "BaseProject.h"
#include "../physics/rigidbodies/ContactInfo.h"

struct Body;

class RigidbodiesProject : public BaseProject
{
public:
    RigidbodiesProject();

    void Setup() override;
    void Input() override;
    void Update() override;
    void FixedUpdate() override;
    void Render() override;
    void Cleanup() override;

private:
    std::vector<Body*> _bodies;
    ContactInfo _contactInfo;
    bool _isCollision;
};
