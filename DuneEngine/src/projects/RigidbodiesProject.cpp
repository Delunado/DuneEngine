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
    Body* circle1 = new Body(CircleShape(100.0f), Vec2(400.0f, 300.0f), 2.0f * 10.0f);
    _bodies.push_back(circle1);

    Body* circle2 = new Body(CircleShape(60.0f), Vec2(500.0f, 300.0f), 2.0f * 6.0f);
    _bodies.push_back(circle2);

    Body* circle3 = new Body(CircleShape(80.0f), Vec2(600.0f, 300.0f), 2.0f * 8.0f);
    _bodies.push_back(circle3);

    Body* circle4 = new Body(CircleShape(40.0f), Vec2(700.0f, 300.0f), 2.0f * 4.0f);
    _bodies.push_back(circle4);

    /*Body* box = new Body(BoxShape(100.0f, 50.0f), Vec2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f), 5.0f);
    _bodies.push_back(box);

    Body* triangle = new Body(PolygonShape({Vec2(0.0f, -50.0f), Vec2(50.0f, 50.0f), Vec2(-50.0f, 50.0f)}),
                              Vec2(600.0f, 300.0f), 5.0f);
    _bodies.push_back(triangle);*/
}

void RigidbodiesProject::Input()
{
    Vector2 mousePosition = GetMousePosition();
    _bodies[0]->position = Vec2(mousePosition.x, mousePosition.y);
}

void RigidbodiesProject::Update()
{
}

void RigidbodiesProject::FixedUpdate()
{
    for (Body* body : _bodies)
    {
        //Vec2 dragForce = Force::GenerateDragForce(*body, 0.005f);
        //body->AddForce(dragForce);

        //Vec2 weightForce = Vec2(0, 9.8f * PIXELS_PER_METER) * body->mass;
        //body->AddForce(weightForce);

        //Vec2 windForce = Vec2(100.0f * PIXELS_PER_METER, 10.0f * PIXELS_PER_METER);
        //body->AddForce(windForce);

        /*float torque = 15.0f * PIXELS_PER_METER;
        body->AddTorque(torque);*/

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
                _contactInfo = contactInfo;
                _isCollision = true;
                body->isColliding = true;
                otherBody->isColliding = true;
                CollisionResolution::ResolvePenetration(contactInfo);
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
        Color color = body->isColliding ? RED : GREEN;

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

    if (!_isCollision) return;

    DrawCircle(_contactInfo.start.x, _contactInfo.start.y, 2.0f, YELLOW);
    DrawCircle(_contactInfo.end.x, _contactInfo.end.y, 2.0f, GREEN);
    DrawLine(_contactInfo.start.x, _contactInfo.start.y, _contactInfo.start.x + _contactInfo.normal.x * 20.0f,
             _contactInfo.start.y + _contactInfo.normal.y * 20.0f, GREEN);
}

void RigidbodiesProject::Cleanup()
{
}
