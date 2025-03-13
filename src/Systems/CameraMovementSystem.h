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
        float zoomScale = camera->GetOrthoScale();

        for (auto &entity: GetEntities()) {
            auto &transform = entity.GetComponent<TransformComponent>();

            float cameraX = transform.position.x; //- camera->GetScreenWidth() / (2.0f * camera->GetOrthoScale());
            float cameraY = transform.position.y; //- camera->GetScreenHeight() / (2.0f * camera->GetOrthoScale());

            // float viewWidthInUnits = camera->GetScreenWidth() / camera->GetOrthoScale();
            // float viewHeightInUnits = camera->GetScreenHeight() / camera->GetOrthoScale();

            // cameraX = std::max(0.0f, std::min(cameraX, static_cast<float>(Game::_mapWidth) - viewWidthInUnits));
            // cameraY = std::max(0.0f, std::min(cameraY, static_cast<float>(Game::_mapHeight) - viewHeightInUnits));

            camera->SetPosition(cameraX, cameraY);
        }
    }
};

#endif //CAMERAMOVEMENTSYSTEM_H
