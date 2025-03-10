#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

struct SpriteComponent {
    std::string id;
    int width;
    int height;
    int zIndex;
    bool isFixed;
    SDL_Rect srcRect;

    explicit SpriteComponent(std::string id = "", const int width = 0, const int height = 0, const int zIndex = 0,
                             const bool isFixed = false,
                             const int srcX = 0, const int srcY = 0)
        : id(id), width(width), height(height), zIndex(zIndex), isFixed(isFixed), srcRect({srcX, srcY, width, height}) {
    }
};

#endif //SPRITECOMPONENT_H
