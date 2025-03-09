#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "System.h"
#include "Entity.h"

#include "BoxColliderComponent.h"

#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class DamageSystem : public System {
public:
    DamageSystem() {
        RequireComponent<BoxColliderComponent>();
    };

    void SubscribeToEvents(const std::unique_ptr<EventBus> &eventBus) {
        eventBus->Subscribe<CollisionEvent>(this, &DamageSystem::OnCollision);
    }

    void OnCollision(CollisionEvent &event) {
        Logger::Log(
            "Damage Between: " + std::to_string(event.entityA.GetId()) + " and " +
            std::to_string(event.entityB.GetId()));
        event.entityA.Kill();
        event.entityB.Kill();
    }

    void Update() const {
    }
};

#endif //DAMAGESYSTEM_H
