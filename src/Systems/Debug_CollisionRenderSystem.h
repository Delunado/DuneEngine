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

    void Update(SDL_Renderer *renderer, SDL_Rect &camera) const {
        for (auto &entity: GetEntities()) {
            auto &transform = entity.GetComponent<TransformComponent>();
            auto &collider = entity.GetComponent<BoxColliderComponent>();

            SDL_Rect rect = {
                static_cast<int>(transform.position.x + collider.offset.x - camera.x),
                static_cast<int>(transform.position.y + collider.offset.y - camera.y),
                collider.width,
                collider.height
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
