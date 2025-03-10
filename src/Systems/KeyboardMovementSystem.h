#ifndef KEYBOARDMOVEMENTSYSTEM_H
#define KEYBOARDMOVEMENTSYSTEM_H

#include "System.h"

#include "../Events/KeyPressedEvent.h"
#include "../Events/KeyReleasedEvent.h"
#include "RigidbodyComponent.h"
#include "MovementByInputComponent.h"

class KeyboardMovementSystem : public System {
public:
    KeyboardMovementSystem() {
        RequireComponent<MovementByInputComponent>();
        RequireComponent<RigidbodyComponent>();
    }

    void SubscribeToEvents(const std::unique_ptr<EventBus> &eventBus) {
        eventBus->Subscribe<KeyPressedEvent>(this, &KeyboardMovementSystem::OnKeyPressed);
        eventBus->Subscribe<KeyReleasedEvent>(this, &KeyboardMovementSystem::OnKeyReleased);
    }

    void OnKeyPressed(KeyPressedEvent &event) {
        for (auto &entity: GetEntities()) {
            auto &rigidbody = entity.GetComponent<RigidbodyComponent>();
            switch (event.symbol) {
                case SDLK_w:
                    rigidbody.velocity.y = -250;
                    break;

                case SDLK_s:
                    rigidbody.velocity.y = 250;
                    break;

                case SDLK_a:
                    rigidbody.velocity.x = -250;
                    break;

                case SDLK_d:
                    rigidbody.velocity.x = 250;
                    break;
            }
        }
    }

    void OnKeyReleased(KeyReleasedEvent &event) {
        for (auto &entity: GetEntities()) {
            auto &rigidbody = entity.GetComponent<RigidbodyComponent>();
            switch (event.symbol) {
                case SDLK_w:
                    if (rigidbody.velocity.y < 0)
                        rigidbody.velocity.y = 0;

                    break;

                case SDLK_s:
                    if (rigidbody.velocity.y > 0)
                        rigidbody.velocity.y = 0;
                    break;

                case SDLK_a:
                    if (rigidbody.velocity.x < 0)
                        rigidbody.velocity.x = 0;
                    break;

                case SDLK_d:
                    if (rigidbody.velocity.x > 0)
                        rigidbody.velocity.x = 0;
                    break;
            }
        }
    }

    void Update() {
    }
};

#endif //KEYBOARDMOVEMENTSYSTEM_H
