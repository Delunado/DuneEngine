#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <algorithm>

#include "System.h"

#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "../Assets/AssetDatabase.h"
#include "../Core/Camera.h"

class RenderSystem : public System {
public:
    RenderSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    };

    void Update(SDL_Renderer *renderer, const std::unique_ptr<Camera> &camera,
                const std::unique_ptr<AssetDatabase> &assetDatabase) {
        OrderEntitiesByZIndex();
        Render(renderer, camera, assetDatabase);
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

    void Render(SDL_Renderer *renderer, const std::unique_ptr<Camera> &camera,
                const std::unique_ptr<AssetDatabase> &assetDatabase) const {
        const float cameraScale = camera->GetOrthoScale();

        for (const auto &entity: _renderableEntities) {
            const auto transform = entity.transform;
            const auto sprite = entity.sprite;

            glm::vec2 screenPos;

            if (sprite.isFixed) {
                screenPos = glm::vec2(transform.position.x, transform.position.y);
            } else {
                screenPos = camera->WorldToScreen(transform.position);
            }

            float spriteWorldWidth = sprite.width / static_cast<float>(sprite.PPU);
            float spriteWorldHeight = sprite.height / static_cast<float>(sprite.PPU);

            int screenWidth = static_cast<int>(spriteWorldWidth * transform.scale.x * cameraScale);
            int screenHeight = static_cast<int>(spriteWorldHeight * transform.scale.y * cameraScale);

            SDL_Rect dstRect = {
                static_cast<int>(screenPos.x - (screenWidth / 2.0f)),
                static_cast<int>(screenPos.y - (screenHeight / 2.0f)),
                screenWidth,
                screenHeight
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
