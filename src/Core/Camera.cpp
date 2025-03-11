#include "Camera.h"

Camera::Camera(const int width, const int height, const float PPU,
               const float orthoSize): _camera({0, 0, width, height}), _PPU(PPU), _orthoSize(orthoSize) {
}

void Camera::SetPosition(const int x, const int y) {
    _camera.x = x;
    _camera.y = y;
}

void Camera::SetX(const int x) {
    _camera.x = x;
}

void Camera::SetY(const int y) {
    _camera.y = y;
}

void Camera::SetSize(const int width, const int height) {
    _camera.w = width;
    _camera.h = height;
}

void Camera::SetPPU(float PPU) {
    if (PPU <= 0)
        PPU = 0.1f;

    _PPU = PPU;
}

void Camera::SetOrthoSize(float orthoSize) {
    if (orthoSize <= 0)
        orthoSize = 0.1f;

    _orthoSize = orthoSize;
}

glm::vec2 Camera::GetPosition() const {
    return glm::vec2(_camera.x, _camera.y);
}

glm::vec2 Camera::GetSize() const {
    return glm::vec2(_camera.w, _camera.h);
}

int Camera::GetX() const {
    return _camera.x;
}

int Camera::GetY() const {
    return _camera.y;
}

int Camera::GetWidth() const {
    return _camera.w;
}

int Camera::GetHeight() const {
    return _camera.h;
}

float Camera::GetPPU() const {
    return _PPU;
}

float Camera::GetOrthoSize() const {
    return _orthoSize;
}

float Camera::GetZoomScale() const {
    return (_camera.h / 2.0f) / (_orthoSize * _PPU);
}
