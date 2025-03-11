#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>
#include <vec2.hpp>

class Camera {
public:
    Camera(int width, int height, float PPU = 16.0f);

    void SetPosition(int x, int y);

    void SetX(int x);

    void SetY(int y);

    void SetSize(int width, int height);

    void SetPPU(float PPU);

    glm::vec2 GetPosition() const;

    glm::vec2 GetSize() const;

    int GetX() const;

    int GetY() const;

    int GetWidth() const;

    int GetHeight() const;

    float GetPPU() const;

private:
    SDL_Rect _camera;
    float _PPU;
};

#endif //CAMERA_H
