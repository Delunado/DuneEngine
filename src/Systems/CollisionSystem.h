#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "Entity.h"
#include "System.h"

#include "BoxColliderComponent.h"
#include "TransformComponent.h"

class CollisionSystem : public System {
public:
    CollisionSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxColliderComponent>();
    };

    void Update() const {
        auto entities = GetEntities();

        for (auto &entity: entities) {
            auto &collider = entity.GetComponent<BoxColliderComponent>();
            collider.isColliding = false;
        }

        for (auto i = entities.begin(); i != entities.end(); ++i) {
            Entity entityA = *i;
            auto &transformA = entityA.GetComponent<TransformComponent>();
            auto &colliderA = entityA.GetComponent<BoxColliderComponent>();

            for (auto j = i; j != entities.end(); ++j) {
                Entity entityB = *j;

                if (entityA == entityB)
                    continue;

                auto &transformB = entityB.GetComponent<TransformComponent>();
                auto &colliderB = entityB.GetComponent<BoxColliderComponent>();

                bool collisionDetected = CheckAABBCollision(transformA.position + colliderA.offset,
                                                            glm::vec2(colliderA.width, colliderA.height),
                                                            transformB.position + colliderB.offset,
                                                            glm::vec2(colliderB.width, colliderB.height));

                if (collisionDetected) {
                    Logger::Log("Collision detected!");
                    colliderA.isColliding = true;
                    colliderB.isColliding = true;

                    entityA.Kill();
                    entityB.Kill();
                }
            }
        }
    }

    bool CheckAABBCollision(const glm::vec2 positionA, const glm::vec2 sizeA, const glm::vec2 positionB,
                            const glm::vec2 sizeB) const {
        return positionA.x < positionB.x + sizeB.x &&
               positionA.x + sizeA.x > positionB.x &&
               positionA.y < positionB.y + sizeB.y &&
               positionA.y + sizeA.y > positionB.y;
    }
};

#endif //COLLISIONSYSTEM_H
