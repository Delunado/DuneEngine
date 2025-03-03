#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "System.h"

#include "TransformComponent.h"
#include "SpriteComponent.h"

class RenderSystem : public System {
public:
    RenderSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    };

    void Update(SDL_Renderer *renderer) const {
        for (auto &entity: GetEntities()) {
            const auto transform = entity.GetComponent<TransformComponent>();
            const auto sprite = entity.GetComponent<SpriteComponent>();

            SDL_Rect rect = {
                static_cast<int>(transform.position.x), static_cast<int>(transform.position.y), sprite.width,
                sprite.height
            };

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
};

#endif //RENDERSYSTEM_H
