#ifndef CAMERAMOVEMENTSYSTEM_H
#define CAMERAMOVEMENTSYSTEM_H

#include "System.h"

#include "TransformComponent.h"
#include "CameraFollowComponent.h"
#include "../Core/Camera.h"

class CameraMovementSystem : public System {
public:
    CameraMovementSystem() {
        RequireComponent<CameraFollowComponent>();
        RequireComponent<TransformComponent>();
    }

    void Update(const std::unique_ptr<Camera> &camera) {
        for (auto &entity: GetEntities()) {
            auto &transform = entity.GetComponent<TransformComponent>();

            if (transform.position.x + (camera->GetWidth() / 2) < Game::_mapWidth) {
                camera->SetX(transform.position.x - (Game::_windowWidth / 2));
            }

            if (transform.position.y + (camera->GetHeight() / 2) < Game::_mapHeight) {
                camera->SetY(transform.position.y - (Game::_windowHeight / 2));
            }

            camera->SetX(camera->GetX() < 0 ? 0 : camera->GetX());
            camera->SetY(camera->GetY() < 0 ? 0 : camera->GetY());
            camera->SetX(camera->GetX() > camera->GetWidth() ? camera->GetWidth() : camera->GetX());
            camera->SetY(camera->GetY() > camera->GetHeight() ? camera->GetHeight() : camera->GetY());
        }
    }
};

#endif //CAMERAMOVEMENTSYSTEM_H
