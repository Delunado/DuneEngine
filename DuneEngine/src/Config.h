#pragma once

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;

inline int MAX_FPS = 60;
inline int FIXED_FPS = 60;
inline float fixedDeltaTime = 1.0f / static_cast<float>(FIXED_FPS);

constexpr int PIXELS_PER_METER = 100;

// PHYSICS
inline float MIN_VELOCITY = 0.01f * PIXELS_PER_METER;
inline float MAX_VELOCITY = 10.0f * PIXELS_PER_METER;

inline float MIN_GRAVITATIONAL_DISTANCE = 3.0f;
inline float MAX_GRAVITATIONAL_DISTANCE = 150.0f;
