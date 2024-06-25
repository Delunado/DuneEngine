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

ConstraintsProject::ConstraintsProject()
{
}

void ConstraintsProject::Setup()
{
    _world = new World(Vec2(0.0f, -9.8f));

    float centerX = WINDOW_WIDTH / 2.0f;
    float centerY = WINDOW_HEIGHT / 2.0f;

    Body* a = new Body(CircleShape(30.0f), Vec2(centerX, centerY), 0.0f);
    Body* b = new Body(CircleShape(20.0f), Vec2(centerX - 100.0f, centerY), 1.0f);
    _world->AddBody(a);
    _world->AddBody(b);

    JointConstraint* joint = new JointConstraint(a, b, a->position);
    _world->AddConstraint(joint);
}

void ConstraintsProject::Input()
{
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
}

void ConstraintsProject::Cleanup()
{
}
