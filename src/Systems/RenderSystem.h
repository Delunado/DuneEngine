#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "System.h"

#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "../Assets/AssetDatabase.h"

class RenderSystem : public System {
public:
    RenderSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    };

    void Update(SDL_Renderer *renderer, std::unique_ptr<AssetDatabase> &assetDatabase) const {
        for (auto &entity: GetEntities()) {
            const auto transform = entity.GetComponent<TransformComponent>();
            const auto sprite = entity.GetComponent<SpriteComponent>();

            SDL_Rect dstRect = {
                static_cast<int>(transform.position.x),
                static_cast<int>(transform.position.y),
                static_cast<int>(sprite.width * transform.scale.x),
                static_cast<int>(sprite.height * transform.scale.y)
            };

            SDL_RenderCopyEx(renderer,
                             assetDatabase->GetTexture(sprite.id),
                             &sprite.srcRect,
                             &dstRect,
                             transform.rotation,
                             nullptr,
                             SDL_FLIP_NONE
            );
        }
    }
};

#endif //RENDERSYSTEM_H
