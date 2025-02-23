#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include <vec2.hpp>


struct TransformComponent {
    glm::vec2 position;
    double rotation;
    glm::vec2 scale;
};


#endif //TRANSFORMCOMPONENT_H
