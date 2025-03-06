#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <algorithm>

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
        // Sort by zIndex.
        // POSSIBLE OPTIMIZATION: Only sort when a new entity is added. Mark as dirty and order here if dirty.
        struct RenderableEntity {
            TransformComponent transform;
            SpriteComponent sprite;
        };

        std::vector<RenderableEntity> renderableEntities;

        for (auto &entity: GetEntities()) {
            renderableEntities.push_back({
                entity.GetComponent<TransformComponent>(),
                entity.GetComponent<SpriteComponent>()
            });
        }

        std::sort(renderableEntities.begin(), renderableEntities.end(),
                  [](const RenderableEntity &entity1, const RenderableEntity &entity2) {
                      return entity1.sprite.zIndex < entity2.sprite.zIndex;
                  });

        // RENDER
        for (const auto &entity: renderableEntities) {
            const auto transform = entity.transform;
            const auto sprite = entity.sprite;

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
