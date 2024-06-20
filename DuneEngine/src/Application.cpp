#include "Application.h"

#include <raylib.h>
#include <vector>

#include "Config.h"
#include "projects/BouncyBallsProject.h"
#include "projects/RigidbodiesProject.h"
#include "render/DURenderer.h"
#include "render/DUShader.h"

struct Particle;

Application::Application()
{
    _running = true;
    _currentProject = new RigidbodiesProject();
}

Application::~Application()
= default;

bool Application::IsRunning() const
{
    return _running;
}

void Application::Setup()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Dune Engine");
    SetTargetFPS(MAX_FPS);

    _stepAccumulator = 0.0f;

    _bgColor = {45, 45, 45, 255};

    _scanlinesShader = DUShader(nullptr, "./shaders/scanlines.glsl");
    _scanlinesShader.SetFloat("renderWidth", WINDOW_WIDTH);
    _scanlinesShader.SetFloat("renderHeight", WINDOW_HEIGHT);

    _renderTarget = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);

    _currentProject->Setup();
}

void Application::Input()
{
    _currentProject->Input();
}

void Application::Update()
{
    _running = !WindowShouldClose();

    float deltaTime = GetFrameTime();
    deltaTime = fminf(deltaTime, 0.25f);

    _stepAccumulator += deltaTime;

    _currentProject->Update(deltaTime);
}

void Application::FixedUpdate()
{
    while (_stepAccumulator >= fixedDeltaTime)
    {
        _currentProject->FixedUpdate(fixedDeltaTime);

        _stepAccumulator -= fixedDeltaTime;
    }
}

void Application::Render()
{
    const float time = GetTime();
    _scanlinesShader.SetFloat("time", time);

    BeginTextureMode(_renderTarget);
    ClearBackground(_bgColor);

    _currentProject->Render();

    EndTextureMode();

    BeginDrawing();

    DURenderer::BeginShaderMode(_scanlinesShader);

    const Rectangle sourceRec = {
        0.0f, 0.0f, static_cast<float>(_renderTarget.texture.width), static_cast<float>(-_renderTarget.texture.height)
    };

    constexpr Vector2 origin = {0.0f, 0.0f};

    DrawTextureRec(_renderTarget.texture, sourceRec, origin, WHITE);

    DURenderer::EndShaderMode();

    DrawFPS(10, 10);

    EndDrawing();
}

void Application::Cleanup() const
{
    _currentProject->Cleanup();

    CloseWindow();
}
