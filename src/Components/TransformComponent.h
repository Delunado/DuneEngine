#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include <vec2.hpp>


struct TransformComponent {
    glm::vec2 position;
    double rotation;
    glm::vec2 scale;

    TransformComponent(glm::vec2 position = glm::vec2(0.0f, 0.0f), double rotation = 0.0f, glm::vec2 scale = glm::vec2(1.0f, 1.0f))
        : position(position), rotation(rotation), scale(scale) {}
};


#endif //TRANSFORMCOMPONENT_H
