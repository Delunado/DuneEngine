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

    //Create an hexagon. Create a polygon with the 6 vertices, counter-clockwise
    Body* hexagon = new Body(PolygonShape({
                                 Vec2(0.0f, -45.0f), Vec2(43.3f, -25.0f), Vec2(43.3f, 25.0f),
                                 Vec2(0.0f, 50.0f), Vec2(-49.3f, 25.0f), Vec2(-36.3f, -25.0f)
                             }),
                             Vec2(centerX, centerY), 10.0f);
    _bodies.push_back(hexagon);

    // Create a static floor
    Body* floor = new Body(BoxShape(WINDOW_WIDTH, 50.0f), Vec2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 35.0f), 0.0f);
    floor->restitution = 0.2f;
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
    ceiling->restitution = 0.7f;
    _bodies.push_back(ceiling);

    // Create a static box in the middle
    Body* boxD = new Body(BoxShape(100.0f, 100.0f), Vec2(centerX, centerY), 0.0f);
    boxD->restitution = 1.0f;
    boxD->friction = 0.4f;
    boxD->rotation = 0.5f;
    _bodies.push_back(boxD);

    // Box 1 - Top left
    Body* box1 = new Body(BoxShape(100.0f, 50.0f), Vec2(130.0f, 150.0f), 0.0f);
    box1->restitution = 0.8f;
    box1->friction = 0.3f;
    box1->rotation = 0.1f;
    _bodies.push_back(box1);

    // Box 2 - Top right
    Body* box2 = new Body(BoxShape(80.0f, 120.0f), Vec2(WINDOW_WIDTH - 170.0f, 180.0f), 0.0f);
    box2->restitution = 0.8f;
    box2->friction = 0.3f;
    box2->rotation = -0.2f;
    _bodies.push_back(box2);

    // Box 3 - Bottom left
    Body* box3 = new Body(BoxShape(100.0f, 100.0f), Vec2(160.0f, WINDOW_HEIGHT - 140.0f), 0.0f);
    box3->restitution = 0.8f;
    box3->friction = 0.3f;
    box3->rotation = 0.4f;
    _bodies.push_back(box3);

    // Box 4 - Bottom right
    Body* box4 = new Body(BoxShape(90.0f, 90.0f), Vec2(WINDOW_WIDTH - 160.0f, WINDOW_HEIGHT - 160.0f), 0.0f);
    box4->restitution = 0.8f;
    box4->friction = 0.3f;
    box4->rotation = -0.5f;
    _bodies.push_back(box4);

    // Box 5 - Center left
    Body* box5 = new Body(BoxShape(70.0f, 110.0f), Vec2(200.0f, centerY - 30.0f), 0.0f);
    box5->restitution = 0.8f;
    box5->friction = 0.3f;
    box5->rotation = -0.3f;
    _bodies.push_back(box5);

    // Box 6 - Center right
    Body* box6 = new Body(BoxShape(100.0f, 70.0f), Vec2(WINDOW_WIDTH - 200.0f, centerY + 30.0f), 0.0f);
    box6->restitution = 0.8f;
    box6->friction = 0.3f;
    box6->rotation = 0.2f;
    _bodies.push_back(box6);

    // Box 7 - Middle upper
    Body* box7 = new Body(BoxShape(100.0f, 50.0f), Vec2(centerX + 100.0f, centerY - 150.0f), 0.0f);
    box7->restitution = 0.8f;
    box7->friction = 0.3f;
    box7->rotation = 0.3f;
    _bodies.push_back(box7);

    // Box 8 - Middle lower
    Body* box8 = new Body(BoxShape(50.0f, 100.0f), Vec2(centerX - 100.0f, centerY + 200.0f), 0.0f);
    box8->restitution = 0.8f;
    box8->friction = 0.3f;
    box8->rotation = -0.4f;
    _bodies.push_back(box8);
}

void RigidbodiesProject::Input()
{
    Vector2 mousePosition = GetMousePosition();

    //Follow mouse position the body 0
    //_bodies[0]->position = Vec2(mousePosition.x, mousePosition.y);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        int random = GetRandomValue(0, 1);

        if (random == 0)
        {
            // Create a box with random size
            float width = GetRandomValue(25, 75);
            float height = GetRandomValue(25, 75);

            Body* box = new Body(BoxShape(width, height), Vec2(mousePosition.x, mousePosition.y), 5.0f);
            box->rotation = GetRandomValue(0, PI * 2.0f);
            box->friction = 0.7f;
            _bodies.push_back(box);
        }
        else
        {
            // Create a ball
            Body* circle = new Body(CircleShape(GetRandomValue(15, 30)), Vec2(mousePosition.x, mousePosition.y), 5.0f);
            circle->restitution = 0.9f;
            circle->friction = 0.15f;

            Texture2D texture = LoadTexture("assets/ball.png");
            circle->SetTexture(&texture);

            _bodies.push_back(circle);
        }
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
        Vec2 dragForce = Force::GenerateDragForce(*body, 0.015f);
        body->AddForce(dragForce);

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
        if (body->GetTexture() != nullptr)
        {
            Vector2 position;
            position.x = body->position.x - body->GetTexture()->width / 2.0f;
            position.y = body->position.y - body->GetTexture()->height / 2.0f;

            DrawTextureEx(*body->GetTexture(), position, body->rotation * RAD2DEG, 1.0f, WHITE);
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
