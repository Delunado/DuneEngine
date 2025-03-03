#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

struct SpriteComponent {
    int width;
    int height;

    SpriteComponent(const int width = 0, const int height = 0)
        : width(width), height(height) {
    }
};

#endif //SPRITECOMPONENT_H
