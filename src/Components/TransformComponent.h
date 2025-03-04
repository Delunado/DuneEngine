#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include <vec2.hpp>

struct TransformComponent {
    glm::vec2 position;
    glm::vec2 scale;
    double rotation;

    TransformComponent(glm::vec2 position = glm::vec2(0.0f, 0.0f), glm::vec2 scale = glm::vec2(1.0f, 1.0f),
                       double rotation = 0.0f)
        : position(position), scale(scale), rotation(rotation) {
    }
};


#endif //TRANSFORMCOMPONENT_H
