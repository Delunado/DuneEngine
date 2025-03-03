#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H
#include <vec2.hpp>

struct RigidbodyComponent {
    glm::vec2 velocity;

    explicit RigidbodyComponent(const glm::vec2 velocity = glm::vec2(0.0f, 0.0f))
        : velocity(velocity) {
    }
};

#endif //RIGIDBODYCOMPONENT_H
