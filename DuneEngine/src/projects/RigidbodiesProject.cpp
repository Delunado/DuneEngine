#include "RigidbodiesProject.h"

#include <iostream>
#include <raylib.h>

#include "../physics/rigidbodies/Body.h"
#include "../physics/rigidbodies/shapes/CircleShape.h"
#include "../Config.h"
#include "../physics/Force.h"
#include "../physics/rigidbodies/CollisionDetection.h"
#include "../physics/rigidbodies/ContactInfo.h"
#include "../physics/rigidbodies/shapes/BoxShape.h"
#include "../render/DUDraw.h"

RigidbodiesProject::RigidbodiesProject()
{
}

void RigidbodiesProject::Setup()
{
    float centerX = WINDOW_WIDTH / 2.0f;
    float centerY = WINDOW_HEIGHT / 2.0f;

    // Box A
    Body* boxA = new Body(BoxShape(50.0f, 50.0f), Vec2(centerX - 300.0f, centerY - 200.0f), 5.0f);
    _bodies.push_back(boxA);
    boxA->angularVelocity = 0.0f;

    // Box B
    Body* boxB = new Body(BoxShape(200.0f, 100.0f), Vec2(centerX + 300.0f, centerY - 200.0f), 5.0f);
    _bodies.push_back(boxB);
    boxB->angularVelocity = 0.1f;

    // Box C
    Body* boxC = new Body(BoxShape(150.0f, 80.0f), Vec2(centerX - 300.0f, centerY + 200.0f), 5.0f);
    _bodies.push_back(boxC);
    boxC->angularVelocity = 5.0f;

    // Define the height of the equilateral triangle based on side length
    float sideLength = 100.0f;
    float height = (sqrt(3.0f) / 2.0f) * sideLength;

    // Equilateral Triangle
    Body* triangle = new Body(PolygonShape({
        Vec2(-sideLength / 2.0f, -height / 3.0f),  // Bottom left vertex
        Vec2(sideLength / 2.0f, -height / 3.0f),   // Bottom right vertex
        Vec2(0.0f, 2.0f * height / 3.0f)           // Top vertex
    }), Vec2(centerX + 300.0f, centerY + 200.0f), 5.0f);
    _bodies.push_back(triangle);
    triangle->angularVelocity = 1.0f;

    // Pentagon
    Body* pentagon = new Body(PolygonShape({
        Vec2(0.0f, -50.0f), Vec2(47.6f, -15.5f), Vec2(29.4f, 40.4f),
        Vec2(-29.4f, 40.4f), Vec2(-47.6f, -15.5f)
    }), Vec2(centerX, centerY), 5.0f);
    _bodies.push_back(pentagon);
    pentagon->angularVelocity = 0.5f;
}

void RigidbodiesProject::Input()
{
    Vector2 mousePosition = GetMousePosition();

    _bodies[0]->position.x = mousePosition.x;
    _bodies[0]->position.y = mousePosition.y;

    //IF space is pressed, apply random impulse to bodies
    if (IsKeyPressed(KEY_SPACE))
    {
        for (int i = 0; i < _bodies.size(); i++)
        {
            Vec2 impulse = Vec2(GetRandomValue(-800.0f * PIXELS_PER_METER, 800.0f * PIXELS_PER_METER),
                                GetRandomValue(-800.0f * PIXELS_PER_METER, 800.0f * PIXELS_PER_METER));
            _bodies[i]->ApplyImpulse(impulse * _bodies[i]->mass);
        }
    }
}

void RigidbodiesProject::Update()
{
}

void RigidbodiesProject::FixedUpdate()
{
    for (Body* body : _bodies)
    {
        Vec2 dragForce = Force::GenerateDragForce(*body, 0.015f);
        body->AddForce(dragForce);

        body->Update(fixedDeltaTime);
    }

    _isCollision = false;
    ContactInfo contactInfo;

    for (int i = 0; i <= _bodies.size() - 1; i++)
    {
        for (int j = i + 1; j < _bodies.size(); j++)
        {
            Body* body = _bodies[i];
            Body* otherBody = _bodies[j];

            body->isColliding = false;
            otherBody->isColliding = false;

            if (CollisionDetection::IsColliding(body, otherBody, contactInfo))
            {
                //CollisionResolution::ResolveCollision(contactInfo);

                _contactInfo = contactInfo;
                _isCollision = true;
                body->isColliding = true;
                otherBody->isColliding = true;
            }
        }
    }

    for (Body* body : _bodies)
    {
        // Limits screen collision
        if (body->shape->GetType() == CIRCLE)
        {
            CircleShape* circle = dynamic_cast<CircleShape*>(body->shape);

            if (body->position.y + circle->radius > WINDOW_HEIGHT)
            {
                body->position.y = WINDOW_HEIGHT - circle->radius;
                body->velocity.y *= -0.95f;
            }
            else if (body->position.y - circle->radius <= 0.0f)
            {
                body->position.y = circle->radius;
                body->velocity.y *= -0.95f;
            }

            if (body->position.x + circle->radius > WINDOW_WIDTH)
            {
                body->position.x = WINDOW_WIDTH - circle->radius;
                body->velocity.x *= -0.95f;
            }
            else if (body->position.x - circle->radius <
                0.0f)
            {
                body->position.x = circle->radius;
                body->velocity.x *= -0.95f;
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
        Color color = false ? RED : WHITE;

        if (body->shape->GetType() == CIRCLE)
        {
            CircleShape* circle = dynamic_cast<CircleShape*>(body->shape);
            DUDraw::DrawCircleLinesAngle(body->position.x, body->position.y, circle->radius, body->rotation, color);
        }
        else if (body->shape->GetType() == BOX || body->shape->GetType() == POLYGON)
        {
            PolygonShape* polygon = dynamic_cast<PolygonShape*>(body->shape);
            DUDraw::DrawPolygon(body->position, polygon->worldVertices, color);
        }
    }

    // Draw contact info
    if (_isCollision)
    {
        DrawCircle(_contactInfo.start.x, _contactInfo.start.y, 5.0f, GREEN);
        DrawCircle(_contactInfo.end.x, _contactInfo.end.y, 5.0f, GREEN);
    }
}

void RigidbodiesProject::Cleanup()
{
}
