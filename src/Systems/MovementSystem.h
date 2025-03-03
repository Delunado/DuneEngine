#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "Entity.h"
#include "System.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"

class MovementSystem : public System {
public:
    MovementSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<RigidbodyComponent>();
    };

    void Update(float deltaTime) const {
        for (auto &entity: GetEntities()) {
            auto &transform = entity.GetComponent<TransformComponent>();
            const auto &rigidbody = entity.GetComponent<RigidbodyComponent>();

            transform.position += rigidbody.velocity * deltaTime;
        }
    }
};

#endif //MOVEMENTSYSTEM_H
