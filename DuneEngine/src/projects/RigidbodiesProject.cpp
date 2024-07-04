#include "RigidbodiesProject.h"

#include <algorithm>
#include <iostream>
#include <raylib.h>

#include "../physics/rigidbodies/Body.h"
#include "../physics/rigidbodies/shapes/CircleShape.h"
#include "../Config.h"
#include "../physics/World.h"
#include "../physics/rigidbodies/shapes/BoxShape.h"
#include "../render/DUDraw.h"

RigidbodiesProject::RigidbodiesProject()
{
    _world = new World(Vec2(0.0f, -9.8f));
}

void RigidbodiesProject::Setup()
{
    float centerX = WINDOW_WIDTH / 2.0f;
    float centerY = WINDOW_HEIGHT / 2.0f;

    _character = new Body(CircleShape(20.0f), Vec2(centerX, centerY), 2.0f);
    _character->elasticity = 0.2f;
    _world->AddBody(_character);

    Texture2D* characterTexture = new Texture2D(LoadTexture("assets/face.png"));
    _character->SetTexture(characterTexture);

    //Create an hexagon. Create a polygon with the 6 vertices, counter-clockwise
    Body* hexagon = new Body(PolygonShape({
                                 Vec2(0.0f, -45.0f), Vec2(43.3f, -25.0f), Vec2(43.3f, 25.0f),
                                 Vec2(0.0f, 50.0f), Vec2(-49.3f, 25.0f), Vec2(-36.3f, -25.0f)
                             }),
                             Vec2(centerX, centerY), 10.0f);
    _world->AddBody(hexagon);

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

    // Create a static box in the middle
    Body* boxD = new Body(BoxShape(100.0f, 100.0f), Vec2(centerX, centerY), 0.0f);
    boxD->elasticity = 1.0f;
    boxD->friction = 0.4f;
    boxD->rotation = 0.5f;
    _world->AddBody(boxD);

    // Box 1 - Top left
    Body* box1 = new Body(BoxShape(100.0f, 50.0f), Vec2(130.0f, 150.0f), 0.0f);
    box1->elasticity = 0.8f;
    box1->friction = 0.3f;
    box1->rotation = 0.1f;
    _world->AddBody(box1);

    // Box 2 - Top right
    Body* box2 = new Body(BoxShape(80.0f, 120.0f), Vec2(WINDOW_WIDTH - 170.0f, 180.0f), 0.0f);
    box2->elasticity = 0.8f;
    box2->friction = 0.3f;
    box2->rotation = -0.2f;
    _world->AddBody(box2);

    // Box 3 - Bottom left
    Body* box3 = new Body(BoxShape(100.0f, 100.0f), Vec2(160.0f, WINDOW_HEIGHT - 140.0f), 0.0f);
    box3->elasticity = 0.8f;
    box3->friction = 0.3f;
    box3->rotation = 0.4f;
    _world->AddBody(box3);

    // Box 4 - Bottom right
    Body* box4 = new Body(BoxShape(90.0f, 90.0f), Vec2(WINDOW_WIDTH - 160.0f, WINDOW_HEIGHT - 160.0f), 0.0f);
    box4->elasticity = 0.8f;
    box4->friction = 0.3f;
    box4->rotation = -0.5f;
    _world->AddBody(box4);

    // Box 5 - Center left
    Body* box5 = new Body(BoxShape(70.0f, 110.0f), Vec2(200.0f, centerY - 30.0f), 0.0f);
    box5->elasticity = 0.8f;
    box5->friction = 0.3f;
    box5->rotation = -0.3f;
    _world->AddBody(box5);

    // Box 6 - Center right
    Body* box6 = new Body(BoxShape(100.0f, 70.0f), Vec2(WINDOW_WIDTH - 200.0f, centerY + 30.0f), 0.0f);
    box6->elasticity = 0.8f;
    box6->friction = 0.3f;
    box6->rotation = 0.2f;
    _world->AddBody(box6);

    // Box 7 - Middle upper
    Body* box7 = new Body(BoxShape(100.0f, 50.0f), Vec2(centerX + 100.0f, centerY - 150.0f), 0.0f);
    box7->elasticity = 0.8f;
    box7->friction = 0.3f;
    box7->rotation = 0.3f;
    _world->AddBody(box7);

    // Box 8 - Middle lower
    Body* box8 = new Body(BoxShape(50.0f, 100.0f), Vec2(centerX - 100.0f, centerY + 200.0f), 0.0f);
    box8->elasticity = 0.8f;
    box8->friction = 0.3f;
    box8->rotation = -0.4f;
    _world->AddBody(box8);

    //_world->AddForce(Vec2(4.0f, 0.0f));
}

void RigidbodiesProject::Input()
{
    Vector2 mousePosition = GetMousePosition();

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
        float width = GetRandomValue(25, 75);
        float height = GetRandomValue(25, 75);

        Body* box = new Body(BoxShape(width, height), Vec2(mousePosition.x, mousePosition.y), 5.0f);
        box->rotation = GetRandomValue(0, PI * 2.0f);
        box->friction = 0.7f;
        _world->AddBody(box);
    }

    // Move the character
    if (IsKeyDown(KEY_A))
    {
        _character->AddForce(Vec2(-1.0f * PIXELS_PER_METER, 0.0f));
    }

    if (IsKeyDown(KEY_D))
    {
        _character->AddForce(Vec2(1.0f * PIXELS_PER_METER, 0.0f));
    }

    if (IsKeyDown(KEY_W))
    {
        _character->AddForce(Vec2(0.0f, -1.0f * PIXELS_PER_METER));
    }

    if (IsKeyDown(KEY_S))
    {
        _character->AddForce(Vec2(0.0f, 1.0f * PIXELS_PER_METER));
    }
}

void RigidbodiesProject::Update(float dt)
{
}

void RigidbodiesProject::FixedUpdate(float dt)
{
    _world->Update(dt);
}

void RigidbodiesProject::Render()
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
