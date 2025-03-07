#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "Entity.h"
#include "System.h"

#include "SpriteComponent.h"
#include "AnimationComponent.h"

class AnimationSystem : public System {
public:
    AnimationSystem() {
        RequireComponent<SpriteComponent>();
        RequireComponent<AnimationComponent>();
    };

    void Update(const float deltaTime) const {
        for (auto entity: GetEntities()) {
            auto &animation = entity.GetComponent<AnimationComponent>();
            auto &sprite = entity.GetComponent<SpriteComponent>();

            animation.currentFrame = ((SDL_GetTicks() - animation.startTime) * animation.frameSpeedRate / 1000) %
                                     animation.numFrames;

            sprite.srcRect.x = animation.currentFrame * sprite.width;
        }
    }
};

#endif //ANIMATIONSYSTEM_H
