#include "DragonProject.h"
#include <raylib.h>
#include "../Config.h"
#include "../physics/particles/Particle.h"
#include "../physics/Force.h"

DragonProject::DragonProject()
{
}

void DragonProject::Setup()
{
    Particle* particle1 = new Particle(Vec2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f), 2.0f);
    particle1->radius = 14.0f;
    _particles.push_back(particle1);

    // Add some particles
    for (int i = 0; i < 10; i++)
    {
        Particle* particle = new Particle(Vec2(GetRandomValue(0, WINDOW_WIDTH), GetRandomValue(0, WINDOW_HEIGHT)),
                                          2.0f);
        particle->radius = 10.0f;
        _particles.push_back(particle);
    }

    _currentMousePosition = Vec2(0.0f, 0.0f);

    _springConstant = 700.0f;
    _restLength = 20.0f;
}

void DragonProject::Input()
{
    _keyboardForce = Vec2(0.0f, 0.0f);

    if (IsKeyDown(KEY_W))
    {
        _keyboardForce.y -= 16.0f * PIXELS_PER_METER;
    }

    if (IsKeyDown(KEY_S))
    {
        _keyboardForce.y += 16.0f * PIXELS_PER_METER;
    }

    if (IsKeyDown(KEY_A))
    {
        _keyboardForce.x -= 16.0f * PIXELS_PER_METER;
    }

    if (IsKeyDown(KEY_D))
    {
        _keyboardForce.x += 16.0f * PIXELS_PER_METER;
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

void DragonProject::Update()
{
}

void DragonProject::FixedUpdate()
{
    // For each particle, connect it to the next one with a spring
    for (int i = 1; i < _particles.size(); i++)
    {
        Vec2 springForce = Force::GenerateSpringForce(*_particles[i], _particles[i - 1]->position, _restLength,
                                                      _springConstant);
        _particles[i]->AddForce(springForce);
    }

    for (Particle* particle : _particles)
    {
        _particles[0]->AddForce(_keyboardForce);

        Vec2 dragForce = Force::GenerateDragForce(*particle, 0.1f);
        particle->AddForce(dragForce);

        Vec2 weightForce = Vec2(0, 9.8f * PIXELS_PER_METER) * particle->mass;
        particle->AddForce(weightForce);

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
}

void DragonProject::Render()
{
    if (_currentMousePosition.x != 0.0f || _currentMousePosition.y != 0.0f)
    {
        DrawLine(_currentMousePosition.x, _currentMousePosition.y, _particles[0]->position.x, _particles[0]->position.y,
                 RED);
    }

    // Draw all springs
    for (int i = 1; i < _particles.size(); i++)
    {
        DrawLine(_particles[i]->position.x, _particles[i]->position.y, _particles[i - 1]->position.x,
                 _particles[i - 1]->position.y, GREEN);
    }

    for (int i = 0; i < _particles.size(); i++)
    {
        Color color = (i == 0 ? GOLD : MAROON);
        DrawCircle(_particles[i]->position.x, _particles[i]->position.y, _particles[i]->radius, color);
    }
}

void DragonProject::Cleanup()
{
    for (Particle* particle : _particles)
    {
        delete particle;
    }
}
