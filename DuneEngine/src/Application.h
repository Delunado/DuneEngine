#pragma once
#include <raylib.h>
#include <vector>

#include "math/Vec2.h"
#include "render/DUShader.h"

class BaseProject;
using namespace Dune::Renderer;

struct Particle;

class Application
{
public:
    Application();
    ~Application();

    bool IsRunning() const;

    void Setup();

    void Input();
    void Update();
    void FixedUpdate();
    void Render();

    void Cleanup() const;

private:
    // General app variables
    bool _running;

    BaseProject* _currentProject;

    Color _bgColor;
    DUShader _scanlinesShader;
    RenderTexture2D _renderTarget;
    bool _shaderEnabled;

    Texture2D* _cursorTexture;
    Vec2 _cursorPos;

    float _stepAccumulator;
};
