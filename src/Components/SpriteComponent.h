#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

struct SpriteComponent {
    std::string id;
    int width;
    int height;
    SDL_Rect srcRect;

    explicit SpriteComponent(std::string id = "", const int width = 0, const int height = 0)
        : id(id), width(width), height(height), srcRect({0, 0, width, height}) {
    }
};

#endif //SPRITECOMPONENT_H
