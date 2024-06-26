﻿#pragma once
#include <raylib.h>
#include <vector>

#include "../math/Vec2.h"

class DUDraw
{
public:
    static void DrawCircleLinesAngle(int startPosX, int startPosY, float radius, float angle, Color color);
    static void DrawCircleLinesAngle(const Vec2& position, float radius, float angle, Color color);
    static void DrawPolygon(std::vector<Vec2>& vertices, Color color);
    static void DrawPolygon(const Vec2& center, std::vector<Vec2>& vertices, Color color);
    static void DrawTexture(const Texture2D& texture, const Vec2& position, const Vec2& origin, float rotation,
                            float scale, Color tint);
};
