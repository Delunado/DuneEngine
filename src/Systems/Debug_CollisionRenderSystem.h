#ifndef DEBUG_COLLISIONRENDERSYSTEM_H
#define DEBUG_COLLISIONRENDERSYSTEM_H

#include "Entity.h"
#include "System.h"

#include "BoxColliderComponent.h"
#include "TransformComponent.h"

class Debug_CollisionRenderSystem : public System {
public:
    Debug_CollisionRenderSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxColliderComponent>();
    };

    void Update(SDL_Renderer *renderer, const std::unique_ptr<Camera> &camera) const {
        for (auto &entity: GetEntities()) {
            auto &transform = entity.GetComponent<TransformComponent>();
            auto &collider = entity.GetComponent<BoxColliderComponent>();

            float cameraScale = camera->GetOrthoScale();

            SDL_Rect rect = {
                static_cast<int>((transform.position.x - camera->GetX()) * cameraScale),
                static_cast<int>((transform.position.y - camera->GetY()) * cameraScale),
                static_cast<int>(collider.width * transform.scale.x * cameraScale),
                static_cast<int>(collider.height * transform.scale.y * cameraScale)
            };

            if (collider.isColliding)
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

            SDL_RenderDrawRect(renderer, &rect);
        }
    }
};

#endif //DEBUG_COLLISIONRENDERSYSTEM_H
