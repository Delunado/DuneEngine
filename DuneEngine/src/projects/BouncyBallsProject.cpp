#include "BouncyBallsProject.h"

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

BouncyBallsProject::BouncyBallsProject()
{
}

void BouncyBallsProject::Setup()
{
    Body* circle5 = new Body(CircleShape(120.0f), Vec2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 130), 0);
    _bodies.push_back(circle5);
    circle5->restitution = 1.0f;

    /*Body* circle1 = new Body(CircleShape(20.0f), Vec2(100.0f, 100.0f), 2.0f * 2.0f);
    _bodies.push_back(circle1);

    Body* circle2 = new Body(CircleShape(20.0f), Vec2(200.0f, 100.0f), 2.0f * 2.0f);
    _bodies.push_back(circle2);

    Body* circle3 = new Body(CircleShape(20.0f), Vec2(300.0f, 100.0f), 2.0f * 2.0f);
    _bodies.push_back(circle3);

    Body* circle4 = new Body(CircleShape(60.0f), Vec2(150.0f, 300.0f), 0);
    _bodies.push_back(circle4);
    circle4->restitution = 1.0f;

    Body* circle5 = new Body(CircleShape(60.0f), Vec2(350.0f, 300.0f), 0);
    _bodies.push_back(circle5);
    circle5->restitution = 1.0f;*/
}

void BouncyBallsProject::Input()
{
    Vector2 mousePosition = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        float mass = GetRandomValue(10.0f, 40.0f);
        Body* circle = new Body(CircleShape(mass), Vec2(mousePosition.x, mousePosition.y), mass);
        _bodies.push_back(circle);
    }

    //IF space is pressed, apply random impulse to bodies
    if (IsKeyPressed(KEY_SPACE))
    {
        for (int i = 0; i < _bodies.size(); i++)
        {
            Vec2 impulse = Vec2(GetRandomValue(-800.0f * PIXELS_PER_METER, 800.0f * PIXELS_PER_METER),
                                GetRandomValue(-800.0f * PIXELS_PER_METER, 800.0f * PIXELS_PER_METER));
            _bodies[i]->ApplyLinearImpulse(impulse * _bodies[i]->mass);
        }
    }
}

void BouncyBallsProject::Update(float dt)
{
}

void BouncyBallsProject::FixedUpdate(float dt)
{
    for (Body* body : _bodies)
    {
        Vec2 dragForce = Force::GenerateDragForce(*body, 0.015f);
        body->AddForce(dragForce);

        Vec2 weightForce = Vec2(0, 15.0f * PIXELS_PER_METER) * body->mass;
        body->AddForce(weightForce);

        /*Vec2 windForce = Vec2(3.0f * PIXELS_PER_METER, 10.0f * PIXELS_PER_METER);
        body->AddForce(windForce);*/

        /*float torque = 15.0f * PIXELS_PER_METER;
        body->AddTorque(torque);*/

        body->IntegrateForces(dt);
        body->IntegrateVelocities(dt);
    }

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

void BouncyBallsProject::Render()
{
    for (Body*& body : _bodies)
    {
        Color color = body == _bodies[0] ? WHITE : GREEN;

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
}

void BouncyBallsProject::Cleanup()
{
}
