#pragma once
#include <raylib.h>
#include <vector>

#include "math/Vec2.h"
#include "render/DUShader.h"

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

    // Specific app functions
    void ShaderRendering(const RenderTexture2D& target, const Shader& shader);

    // Specific app variables
    Color _bgColor;
    DUShader _scanlinesShader;
    RenderTexture2D _renderTarget;

    float _stepAccumulator;

    Vec2 _currentMousePosition;
    std::vector<Particle*> _particles;

    Vec2 _keyboardForce;
};
