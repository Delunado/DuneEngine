#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>
#include <vec2.hpp>

class Camera {
public:
    Camera(int width, int height, float PPU = 16.0f, float orthoSize = 5.0f);

    void SetPosition(int x, int y);

    void SetX(int x);

    void SetY(int y);

    void SetSize(int width, int height);

    void SetPPU(float PPU);

    void SetOrthoSize(float orthoSize);

    glm::vec2 GetPosition() const;

    glm::vec2 GetSize() const;

    int GetX() const;

    int GetY() const;

    int GetWidth() const;

    int GetHeight() const;

    float GetPPU() const;

    float GetOrthoSize() const;

    float GetZoomScale() const;

private:
    SDL_Rect _camera;
    float _PPU;
    float _orthoSize;
};

#endif //CAMERA_H
