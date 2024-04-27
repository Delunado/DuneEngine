#pragma once

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;

inline int MAX_FPS = -1;
inline int FIXED_FPS = 60;
inline float fixedDeltaTime = 1.0f / static_cast<float>(FIXED_FPS);

constexpr int PIXELS_PER_METER = 100;

inline float MIN_VELOCITY = 0.1f * PIXELS_PER_METER;