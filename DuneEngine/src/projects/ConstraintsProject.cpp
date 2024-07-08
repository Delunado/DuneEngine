#include "ConstraintsProject.h"

#include <algorithm>
#include <iostream>
#include <raylib.h>

#include "../physics/rigidbodies/Body.h"
#include "../physics/rigidbodies/shapes/CircleShape.h"
#include "../Config.h"
#include "../physics/JointConstraint.h"
#include "../physics/World.h"
#include "../physics/rigidbodies/shapes/BoxShape.h"
#include "../render/DUDraw.h"

ConstraintsProject::ConstraintsProject() = default;

void ConstraintsProject::Setup()
{
    _world = new World(Vec2(0.0f, -9.8f * PIXELS_PER_METER));

    float centerX = WINDOW_WIDTH / 2.0f;
    float centerY = WINDOW_HEIGHT / 2.0f;

    // const int NUM_BODIES = 8;
    // for (int i = 0; i < NUM_BODIES; i++)
    // {
    //     float mass = (i == 0) ? 0.0f : 1.0f;
    //     Body* body = new Body(BoxShape(30.0f, 30.0f), Vec2(centerX - i * 40.0f, centerY - 100), mass);
    //     _world->AddBody(body);
    //     _lastJointBody = body;
    // }
    //
    // for (int i = 0; i < NUM_BODIES - 1; i++)
    // {
    //     Body* a = _world->GetBodies()[i];
    //     Body* b = _world->GetBodies()[i + 1];
    //     JointConstraint* joint = new JointConstraint(a, b, a->position);
    //     _world->AddConstraint(joint);
    // }

    // Create a diamond shape
    Body* diamond = new Body(
        PolygonShape({
            Vec2(0.0f, -65.0f), Vec2(70.0f, 0.0f), Vec2(0.0f, 34.0f), Vec2(-70.0f, 0.0f), Vec2(-12.0f, -65.0f)
        }),
        Vec2(centerX, centerY), 1.0f);
    diamond->elasticity = 0.2f;
    _world->AddBody(diamond);

    // Create a static floor
    Body* floor = new Body(BoxShape(WINDOW_WIDTH, 50.0f), Vec2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 35.0f), 0.0f);
    floor->elasticity = 0.2f;
    _world->AddBody(floor);

    // Create a static wall on the left
    Body* wallLeft = new Body(BoxShape(50.0f, WINDOW_HEIGHT - 115), Vec2(25.0f, WINDOW_HEIGHT / 2.02f), 0.0f);
    wallLeft->elasticity = 0.9f;
    _world->AddBody(wallLeft);

    Body* wallRight = new Body(BoxShape(50.0f, WINDOW_HEIGHT - 115), Vec2(WINDOW_WIDTH - 25.0f, WINDOW_HEIGHT / 2.02f),
                               0.0f);
    wallRight->elasticity = 0.9f;
    _world->AddBody(wallRight);

    // Create a static ceiling
    Body* ceiling = new Body(BoxShape(WINDOW_WIDTH, 50.0f), Vec2(WINDOW_WIDTH / 2.0f, 25.0f), 0.0f);
    ceiling->elasticity = 0.7f;
    _world->AddBody(ceiling);
}

void ConstraintsProject::Input()
{
    Vector2 mousePosition = GetMousePosition();

    // If pressing A or D, apply a force to the world in the X axis
    if (IsKeyPressed(KEY_A))
    {
        _lastJointBody->ApplyLinearImpulse(Vec2(-10.0f * PIXELS_PER_METER, 0.0f));
    }

    if (IsKeyPressed(KEY_D))
    {
        _lastJointBody->ApplyLinearImpulse(Vec2(10.0f * PIXELS_PER_METER, 0.0f));
    }

    if (IsKeyPressed(KEY_W))
    {
        _lastJointBody->ApplyLinearImpulse(Vec2(0.0f, -10.0f * PIXELS_PER_METER));
    }

    if (IsKeyPressed(KEY_S))
    {
        _lastJointBody->ApplyLinearImpulse(Vec2(0.0f, 10.0f * PIXELS_PER_METER));
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Create a ball
        Body* circle = new Body(CircleShape(GetRandomValue(15, 30)), Vec2(mousePosition.x, mousePosition.y), 5.0f);
        circle->elasticity = 0.9f;
        circle->friction = 0.15f;

        Texture2D* texture = new Texture2D(LoadTexture("assets/face.png"));
        circle->SetTexture(texture);

        _world->AddBody(circle);
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        // Create a box with random size
        float width = GetRandomValue(75, 75);
        float height = GetRandomValue(75, 75);

        Body* box = new Body(BoxShape(width, height), Vec2(mousePosition.x, mousePosition.y), 5.0f);
        //box->rotation = GetRandomValue(0, PI * 2.0f);
        box->friction = 0.3f;
        box->elasticity = 0.1f;
        _world->AddBody(box);
    }
}

void ConstraintsProject::Update(float dt)
{
}

void ConstraintsProject::FixedUpdate(float dt)
{
    _world->Update(dt);
}

void ConstraintsProject::Render()
{
    // Join every constraint bodies with a line
    for (size_t i = 0; i < _world->GetConstraints().size(); i++)
    {
        Body* bodyA = _world->GetConstraints()[i]->_bodyA;
        Body* bodyB = _world->GetConstraints()[i]->_bodyB;

        DrawLine(bodyA->position.x, bodyA->position.y, bodyB->position.x, bodyB->position.y, RED);
    }

    for (Body*& body : _world->GetBodies())
    {
        if (body->GetTexture() != nullptr)
        {
            if (body->shape->GetType() == CIRCLE)
            {
                CircleShape* circle = dynamic_cast<CircleShape*>(body->shape);

                float scale = (circle->radius / body->GetTexture()->width) * 2.0f;
                Vec2 position = body->position;
                Vec2 origin = Vec2(body->GetTexture()->width * 0.5f, body->GetTexture()->height * 0.5f);

                //DUDraw::DrawCircleLinesAngle(position, circle->radius, body->rotation, WHITE);
                DUDraw::DrawTexture(*body->GetTexture(), position, origin, body->rotation * RAD2DEG, scale,
                                    LIGHTGRAY);
            }
        }
        else
        {
            if (body->shape->GetType() == CIRCLE)
            {
                Color color = RAYWHITE;
                CircleShape* circle = dynamic_cast<CircleShape*>(body->shape);
                DUDraw::DrawCircleLinesAngle(body->position.x, body->position.y, circle->radius, body->rotation, color);
            }
            else if (body->shape->GetType() == BOX || body->shape->GetType() == POLYGON)
            {
                Color color = LIGHTGRAY;
                PolygonShape* polygon = dynamic_cast<PolygonShape*>(body->shape);
                DUDraw::DrawPolygon(body->position, polygon->worldVertices, color);
            }
        }
    }

    _world->DrawDebug();
}

void ConstraintsProject::Cleanup()
{
}
