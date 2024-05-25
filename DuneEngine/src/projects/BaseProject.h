#pragma once

class BaseProject
{
public:
    virtual void Setup() = 0;

    virtual void Input() = 0;
    virtual void Update() = 0;
    virtual void FixedUpdate() = 0;
    virtual void Render() = 0;

    virtual void Cleanup() = 0;
};
