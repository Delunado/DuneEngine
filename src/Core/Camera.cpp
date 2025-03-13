#include "Camera.h"

Camera::Camera(const int screenWidth, const int screenHeight, const float orthoSize): _position({0, 0}),
    _screenSize({screenWidth, screenHeight}),
    _orthoSize(orthoSize) {
}

void Camera::SetPosition(const int x, const int y) {
    _position.x = x;
    _position.y = y;
}

void Camera::SetX(const int x) {
    _position.x = x;
}

void Camera::SetY(const int y) {
    _position.y = y;
}

void Camera::SetScreenSize(const int width, const int height) {
    _screenSize.x = width;
    _screenSize.y = height;
}

void Camera::SetOrthoSize(float orthoSize) {
    if (orthoSize <= 0)
        orthoSize = 0.1f;

    _orthoSize = orthoSize;
}

glm::vec2 Camera::GetPosition() const {
    return glm::vec2(_position.x, _position.y);
}

glm::vec2 Camera::GetSize() const {
    return glm::vec2(_screenSize.x, _screenSize.y);
}

int Camera::GetX() const {
    return _position.x;
}

int Camera::GetY() const {
    return _position.y;
}

int Camera::GetScreenWidth() const {
    return _screenSize.x;
}

int Camera::GetScreenHeight() const {
    return _screenSize.y;
}

float Camera::GetOrthoSize() const {
    return _orthoSize;
}

float Camera::GetOrthoScale() const {
    return _screenSize.y / (2.0f * _orthoSize);
}

glm::vec2 Camera::WorldToScreen(const glm::vec2 &worldPosition) const {
    const float scale = GetOrthoScale();

    return glm::vec2((worldPosition.x - _position.x) * scale + _screenSize.x * 0.5f,
                     (worldPosition.y - _position.y) * scale + _screenSize.y * 0.5f);
}

glm::vec2 Camera::ScreenToWorld(const glm::vec2 &screenPosition) const {
    const float scale = GetOrthoScale();

    return glm::vec2((screenPosition.x - _screenSize.x * 0.5f) / scale + _position.x,
                     (screenPosition.y - _screenSize.y * 0.5f) / scale + _position.y);
}
