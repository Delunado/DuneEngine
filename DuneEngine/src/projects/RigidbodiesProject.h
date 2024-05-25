#pragma once
#include <vector>

#include "BaseProject.h"

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
};
