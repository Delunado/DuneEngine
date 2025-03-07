﻿#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include <glm.hpp>

struct BoxColliderComponent {
    int width;
    int height;
    glm::vec2 offset;
    bool isColliding;

    BoxColliderComponent(const int width = 8, const int height = 8,
                         const glm::vec2 offset = glm::vec2(0, 0)) : width(width),
                                                                     height(height), offset(offset),
                                                                     isColliding(false) {
    }
};

#endif //BOXCOLLIDERCOMPONENT_H
