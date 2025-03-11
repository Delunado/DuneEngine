#include "Camera.h"

Camera::Camera(const int width, const int height): _camera({0, 0, width, height}) {
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
