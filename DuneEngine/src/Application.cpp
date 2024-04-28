#include "Application.h"

#include <raylib.h>
#include <vector>

#include "Config.h"
#include "math/Vec2.h"
#include "physics/Force.h"
#include "physics/Particle.h"

#define PLATFORM_DESKTOP

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif


struct Particle;

Application::Application()
{
    _running = true;
}

Application::~Application()
= default;

bool Application::IsRunning() const
{
    return _running;
}

void Application::Setup()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hardy2D Engine");
    SetTargetFPS(MAX_FPS);

    _stepAccumulator = 0.0f;

    _bgColor = {45, 45, 45, 255};

    _scanlinesShader = LoadShader(0, TextFormat("./shaders/scanlines.glsl", GLSL_VERSION));

    SetShaderValue(_scanlinesShader, GetShaderLocation(_scanlinesShader, "renderWidth"), &WINDOW_WIDTH,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(_scanlinesShader, GetShaderLocation(_scanlinesShader, "renderHeight"), &WINDOW_HEIGHT,
                   SHADER_UNIFORM_FLOAT);

    _renderTarget = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);


    Particle* particle1 = new Particle(Vec2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 4.0f), 1.0f);
    particle1->radius = 5;

    _particles.push_back(particle1);

    _currentMousePosition = Vec2(0.0f, 0.0f);
}

void Application::Input()
{
    _keyboardForce = Vec2(0.0f, 0.0f);

    if (IsKeyDown(KEY_W))
    {
        _keyboardForce.y -= 12.0f * PIXELS_PER_METER;
    }

    if (IsKeyDown(KEY_S))
    {
        _keyboardForce.y += 12.0f * PIXELS_PER_METER;
    }

    if (IsKeyDown(KEY_A))
    {
        _keyboardForce.x -= 12.0f * PIXELS_PER_METER;
    }

    if (IsKeyDown(KEY_D))
    {
        _keyboardForce.x += 12.0f * PIXELS_PER_METER;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        Particle* particle = new Particle(Vec2(GetMousePosition().x, GetMousePosition().y), GetRandomValue(15, 35));
        particle->radius = particle->mass / 1.5f;
        _particles.push_back(particle);
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        _currentMousePosition = Vec2(GetMousePosition().x, GetMousePosition().y);
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        const Vec2 direction = (_particles[0]->position - _currentMousePosition).UnitVector();
        const float force = (_particles[0]->position - _currentMousePosition).Magnitude() * 3.0f;

        _particles[0]->velocity = direction * force;

        _currentMousePosition = Vec2(0.0f, 0.0f);
    }
}

void Application::Update()
{
    _running = !WindowShouldClose();

    float deltaTime = GetFrameTime();
    deltaTime = fminf(deltaTime, 0.25f);

    _stepAccumulator += deltaTime;
}

void Application::FixedUpdate()
{
    while (_stepAccumulator >= fixedDeltaTime)
    {
        _particles[0]->AddForce(_keyboardForce);

        // Compute gravitational forces for particles[0] with all other particles
        for (size_t i = 1; i < _particles.size(); i++)
        {
            Vec2 gravitationalForce = Force::GenerateGravitationalForce(*_particles[0], *_particles[i],
                                                                        50.0f * PIXELS_PER_METER);
            _particles[0]->AddForce(gravitationalForce);
        }

        for (Particle* particle : _particles)
        {
            Vec2 frictionForce = Force::GenerateFrictionForce(*particle, 1.0f * PIXELS_PER_METER);
            particle->AddForce(frictionForce);

            particle->Integrate(fixedDeltaTime);

            // Limits screen collision
            if (particle->position.y + particle->radius > WINDOW_HEIGHT)
            {
                particle->position.y = WINDOW_HEIGHT - particle->radius;
                particle->velocity.y *= -0.99f;
            }
            else if (particle->position.y - particle->radius <= 0.0f)
            {
                particle->position.y = particle->radius;
                particle->velocity.y *= -0.99f;
            }

            if (particle->position.x + particle->radius > WINDOW_WIDTH)
            {
                particle->position.x = WINDOW_WIDTH - particle->radius;
                particle->velocity.x *= -0.99f;
            }
            else if (particle->position.x - particle->radius <
                0.0f)
            {
                particle->position.x = particle->radius;
                particle->velocity.x *= -0.99f;
            }
        }

        _stepAccumulator -= fixedDeltaTime;
    }
}

void Application::Render()
{
    BeginTextureMode(_renderTarget);
    ClearBackground(_bgColor);

    if (_currentMousePosition.x != 0.0f || _currentMousePosition.y != 0.0f)
    {
        DrawLine(_currentMousePosition.x, _currentMousePosition.y, _particles[0]->position.x, _particles[0]->position.y,
                 RED);
    }

    for (int i = 0; i < _particles.size(); i++)
    {
        Color color = i == 0 ? GREEN : RED;

        DrawCircle(_particles[i]->position.x, _particles[i]->position.y, _particles[i]->radius, color);
    }

    EndTextureMode();

    BeginDrawing();
    ShaderRendering(_renderTarget, _scanlinesShader);
    DrawFPS(10, 10);
    EndDrawing();
}

void Application::Cleanup() const
{
    for (Particle* particle : _particles)
    {
        delete particle;
    }

    CloseWindow();
}

// Specific app functions
void Application::ShaderRendering(const RenderTexture2D& target, const Shader& shader)
{
    BeginShaderMode(shader);
    const float time = GetTime();
    SetShaderValue(shader, GetShaderLocation(shader, "time"), &time, SHADER_UNIFORM_FLOAT);

    const Rectangle sourceRec = {
        0.0f, 0.0f, static_cast<float>(target.texture.width), static_cast<float>(-target.texture.height)
    };
    constexpr Vector2 origin = {0.0f, 0.0f};
    DrawTextureRec(target.texture, sourceRec, origin, WHITE);
    EndShaderMode();
}
