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

    void Update(SDL_Renderer *renderer, const std::unique_ptr<AssetDatabase> &assetDatabase) {
        OrderEntitiesByZIndex();
        Render(renderer, assetDatabase);
    }

private:
    struct RenderableEntity {
        TransformComponent transform;
        SpriteComponent sprite;
    };

    // POSSIBLE OPTIMIZATION: Only sort when a new entity is added. Mark as dirty and order here if dirty.
    void OrderEntitiesByZIndex() {
        _renderableEntities.clear();

        for (auto &entity: GetEntities()) {
            _renderableEntities.push_back({
                entity.GetComponent<TransformComponent>(),
                entity.GetComponent<SpriteComponent>()
            });
        }

        std::sort(_renderableEntities.begin(), _renderableEntities.end(),
                  [](const RenderableEntity &entity1, const RenderableEntity &entity2) {
                      return entity1.sprite.zIndex < entity2.sprite.zIndex;
                  });
    }

    void Render(SDL_Renderer *renderer, const std::unique_ptr<AssetDatabase> &assetDatabase) const {
        for (const auto &entity: _renderableEntities) {
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

    std::vector<RenderableEntity> _renderableEntities;
};

#endif //RENDERSYSTEM_H
