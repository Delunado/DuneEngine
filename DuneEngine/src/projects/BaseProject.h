#pragma once

class BaseProject
{
public:
    virtual ~BaseProject() = default;

    virtual void Setup() = 0;

    virtual void Input() = 0;
    virtual void Update(float dt) = 0;
    virtual void FixedUpdate(float dt) = 0;
    virtual void Render() = 0;

    virtual void Cleanup() = 0;
};
