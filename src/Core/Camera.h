#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>
#include <vec2.hpp>

class Camera {
public:
    Camera(int screenWidth, int screenHeight, float orthoSize = 6.0f);

    void SetPosition(int x, int y);

    void SetX(int x);

    void SetY(int y);

    void SetScreenSize(int width, int height);

    void SetOrthoSize(float orthoSize);

    glm::vec2 GetPosition() const;

    glm::vec2 GetSize() const;

    int GetX() const;

    int GetY() const;

    int GetScreenWidth() const;

    int GetScreenHeight() const;

    float GetOrthoSize() const;

    float GetOrthoScale() const;

    glm::vec2 WorldToScreen(const glm::vec2 &worldPosition) const;

    glm::vec2 ScreenToWorld(const glm::vec2 &screenPosition) const;

private:
    glm::vec2 _position;
    glm::ivec2 _screenSize;

    float _orthoSize;
};

#endif //CAMERA_H
