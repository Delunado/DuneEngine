#include "RigidbodiesProject.h"

#include <iostream>
#include <raylib.h>

#include "../physics/rigidbodies/Body.h"
#include "../physics/rigidbodies/shapes/CircleShape.h"
#include "../Config.h"
#include "../physics/Force.h"
#include "../physics/rigidbodies/shapes/BoxShape.h"
#include "../render/DUDraw.h"

RigidbodiesProject::RigidbodiesProject()
{
}

void RigidbodiesProject::Setup()
{
    Body* body = new Body(CircleShape(40.0f), Vec2(400.0f, 300.0f), 5.0f);
    _bodies.push_back(body);

    Body* box = new Body(BoxShape(100.0f, 50.0f), Vec2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f), 5.0f);
    _bodies.push_back(box);

    Body* triangle = new Body(PolygonShape({Vec2(0.0f, -50.0f), Vec2(50.0f, 50.0f), Vec2(-50.0f, 50.0f)}),
                              Vec2(600.0f, 300.0f), 5.0f);
    _bodies.push_back(triangle);
}

void RigidbodiesProject::Input()
{
}

void RigidbodiesProject::Update()
{
}

void RigidbodiesProject::FixedUpdate()
{
    for (Body* body : _bodies)
    {
        Vec2 dragForce = Force::GenerateDragForce(*body, 0.05f);
        body->AddForce(dragForce);

        Vec2 weightForce = Vec2(0, 0.8f * PIXELS_PER_METER) * body->mass;
        //body->AddForce(weightForce);

        float torque = 15.0f * PIXELS_PER_METER;
        body->AddTorque(torque);

        body->IntegrateLinear(fixedDeltaTime);
        body->IntegrateAngular(fixedDeltaTime);

        if (body->shape->GetType() == BOX || body->shape->GetType() == POLYGON)
        {
            PolygonShape* polygon = dynamic_cast<PolygonShape*>(body->shape);
            polygon->UpdateVertices(body->position, body->rotation);
        }

        // Limits screen collision
        if (body->shape->GetType() == CIRCLE)
        {
            CircleShape* circle = dynamic_cast<CircleShape*>(body->shape);

            if (body->position.y + circle->radius > WINDOW_HEIGHT)
            {
                body->position.y = WINDOW_HEIGHT - circle->radius;
                body->velocity.y *= -0.9f;
            }
            else if (body->position.y - circle->radius <= 0.0f)
            {
                body->position.y = circle->radius;
                body->velocity.y *= -0.9f;
            }

            if (body->position.x + circle->radius > WINDOW_WIDTH)
            {
                body->position.x = WINDOW_WIDTH - circle->radius;
                body->velocity.x *= -0.9f;
            }
            else if (body->position.x - circle->radius <
                0.0f)
            {
                body->position.x = circle->radius;
                body->velocity.x *= -0.9f;
            }
        }
        else if (body->shape->GetType() == BOX)
        {
            //TODO
        }
    }
}

void RigidbodiesProject::Render()
{
    for (Body*& body : _bodies)
    {
        if (body->shape->GetType() == CIRCLE)
        {
            CircleShape* circle = dynamic_cast<CircleShape*>(body->shape);
            DUDraw::DrawCircleLinesAngle(body->position.x, body->position.y, circle->radius, body->rotation, RED);
        }
        else if (body->shape->GetType() == BOX || body->shape->GetType() == POLYGON)
        {
            PolygonShape* polygon = dynamic_cast<PolygonShape*>(body->shape);
            DUDraw::DrawPolygon(body->position, polygon->worldVertices, RED);
        }
    }
}

void RigidbodiesProject::Cleanup()
{
}
