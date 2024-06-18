#include "RigidbodiesProject.h"

#include <iostream>
#include <raylib.h>

#include "../physics/rigidbodies/Body.h"
#include "../physics/rigidbodies/shapes/CircleShape.h"
#include "../Config.h"
#include "../physics/Force.h"
#include "../physics/rigidbodies/CollisionDetection.h"
#include "../physics/rigidbodies/CollisionResolution.h"
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
    /*Body* boxA = new Body(BoxShape(50.0f, 50.0f), Vec2(centerX - 300.0f, centerY - 200.0f), 5.0f);
    _bodies.push_back(boxA);

    // Box B
    Body* boxB = new Body(BoxShape(200.0f, 100.0f), Vec2(centerX + 300.0f, centerY - 200.0f), 5.0f);
    _bodies.push_back(boxB);

    // Box C
    Body* boxC = new Body(BoxShape(150.0f, 80.0f), Vec2(centerX - 300.0f, centerY + 200.0f), 5.0f);
    _bodies.push_back(boxC);*/

    // Define the height of the equilateral triangle based on side length
    float sideLength = 100.0f;
    float height = (sqrt(3.0f) / 2.0f) * sideLength;

    // Equilateral Triangle
    Body* triangle = new Body(PolygonShape({
                                  Vec2(-sideLength / 2.0f, -height / 3.0f), // Bottom left vertex
                                  Vec2(sideLength / 2.0f, -height / 3.0f), // Bottom right vertex
                                  Vec2(0.0f, 2.0f * height / 3.0f) // Top vertex
                              }), Vec2(centerX + 300.0f, centerY + 200.0f), 5.0f);
    _bodies.push_back(triangle);
    triangle->rotation = 0.5f;

    // Pentagon
    Body* pentagon = new Body(PolygonShape({
                                  Vec2(0.0f, -50.0f), Vec2(47.6f, -15.5f), Vec2(29.4f, 40.4f),
                                  Vec2(-29.4f, 40.4f), Vec2(-47.6f, -15.5f)
                              }), Vec2(centerX, centerY), 5.0f);
    _bodies.push_back(pentagon);

    // Circle A
    /*Body* circleA = new Body(CircleShape(50.0f), Vec2(centerX - 300.0f, centerY - 200.0f), 5.0f);
    _bodies.push_back(circleA);

    Body* circleB = new Body(CircleShape(100.0f), Vec2(centerX + 300.0f, centerY - 200.0f), 5.0f);
    _bodies.push_back(circleB);*/

    // Create a static floor
    Body* floor = new Body(BoxShape(WINDOW_WIDTH, 50.0f), Vec2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 35.0f), 0.0f);
    floor->restitution = 0.2f;
    floor->friction = 0.7f;
    _bodies.push_back(floor);

    // Create a static wall on the left
    Body* wallLeft = new Body(BoxShape(50.0f, WINDOW_HEIGHT - 115), Vec2(25.0f, WINDOW_HEIGHT / 2.02f), 0.0f);
    wallLeft->restitution = 0.9f;
    _bodies.push_back(wallLeft);

    Body* wallRight = new Body(BoxShape(50.0f, WINDOW_HEIGHT - 115), Vec2(WINDOW_WIDTH - 25.0f, WINDOW_HEIGHT / 2.02f),
                               0.0f);
    wallRight->restitution = 0.9f;
    _bodies.push_back(wallRight);

    // Create a static ceiling
    Body* ceiling = new Body(BoxShape(WINDOW_WIDTH, 50.0f), Vec2(WINDOW_WIDTH / 2.0f, 25.0f), 0.0f);
    ceiling->restitution = 0.2f;
    _bodies.push_back(ceiling);

    // Create a static box in the middle
    Body* boxD = new Body(BoxShape(100.0f, 100.0f), Vec2(centerX, centerY), 0.0f);
    boxD->restitution = 0.8f;
    boxD->friction = 0.4f;
    boxD->rotation = 0.5f;
    _bodies.push_back(boxD);
}

void RigidbodiesProject::Input()
{
    Vector2 mousePosition = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Create a box with random size
        float width = GetRandomValue(25, 75);
        float height = GetRandomValue(25, 75);

        Body* box = new Body(BoxShape(width, height), Vec2(mousePosition.x, mousePosition.y), 5.0f);
        box->rotation = GetRandomValue(0, PI * 2.0f);
        _bodies.push_back(box);

        // Create a ball
        /*Body* circle = new Body(CircleShape(GetRandomValue(10, 50)), Vec2(mousePosition.x, mousePosition.y), 5.0f);
        _bodies.push_back(circle);*/
    }

    //IF space is pressed, apply random impulse to bodies
    if (IsKeyPressed(KEY_SPACE))
    {
        for (auto& _bodie : _bodies)
        {
            Vec2 impulse = Vec2(GetRandomValue(-1200.0f * PIXELS_PER_METER, 1200.0f * PIXELS_PER_METER),
                                GetRandomValue(600.0f * PIXELS_PER_METER, 1200.0f * PIXELS_PER_METER));
            _bodie->ApplyImpulse(impulse * _bodie->mass);
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
        /*Vec2 dragForce = Force::GenerateDragForce(*body, 0.015f);
        body->AddForce(dragForce);*/

        Vec2 weightForce = Vec2(0.0f, body->mass * 9.8f * PIXELS_PER_METER);
        body->AddForce(weightForce);

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
                CollisionResolution::ResolveCollision(contactInfo);

                _contactInfo = contactInfo;
                _isCollision = true;
                body->isColliding = true;
                otherBody->isColliding = true;
            }
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
    /*if (_isCollision)
    {
        DrawCircle(_contactInfo.start.x, _contactInfo.start.y, 5.0f, GREEN);
        DrawCircle(_contactInfo.end.x, _contactInfo.end.y, 5.0f, GREEN);
    }*/
}

void RigidbodiesProject::Cleanup()
{
}
