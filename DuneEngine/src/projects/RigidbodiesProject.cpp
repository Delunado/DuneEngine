#include "RigidbodiesProject.h"

#include <raylib.h>

#include "../physics/rigidbodies/Body.h"
#include "../physics/rigidbodies/shapes/CircleShape.h"
#include "../Config.h"
#include "../physics/Force.h"
#include "../render/DUDraw.h"

RigidbodiesProject::RigidbodiesProject()
{
}

void RigidbodiesProject::Setup()
{
    Body* body = new Body(CircleShape(20.0f), Vec2(400.0f, 300.0f), 15.0f);
    _bodies.push_back(body);
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

        Vec2 weightForce = Vec2(0, 9.8f * PIXELS_PER_METER) * body->mass;
        body->AddForce(weightForce);

        body->Integrate(fixedDeltaTime);

        // Limits screen collision
        if (body->shape->GetType() == CIRCLE)
        {
            CircleShape* circle = dynamic_cast<CircleShape*>(body->shape);

            if (body->position.y + circle->radius > WINDOW_HEIGHT)
            {
                body->position.y = WINDOW_HEIGHT - circle->radius;
                body->velocity.y *= -0.99f;
            }
            else if (body->position.y - circle->radius <= 0.0f)
            {
                body->position.y = circle->radius;
                body->velocity.y *= -0.99f;
            }

            if (body->position.x + circle->radius > WINDOW_WIDTH)
            {
                body->position.x = WINDOW_WIDTH - circle->radius;
                body->velocity.x *= -0.99f;
            }
            else if (body->position.x - circle->radius <
                0.0f)
            {
                body->position.x = circle->radius;
                body->velocity.x *= -0.99f;
            }
        }
    }
}

void RigidbodiesProject::Render()
{
    static float angle = 0.0f;

    for (Body*& body : _bodies)
    {
        if (body->shape->GetType() == CIRCLE)
        {
            CircleShape* circle = dynamic_cast<CircleShape*>(body->shape);
            DUDraw::DrawCircleLinesAngle(body->position.x, body->position.y, circle->radius, angle, RED);
        }
        else
        {
            //TODO
        }
    }
}

void RigidbodiesProject::Cleanup()
{
}
