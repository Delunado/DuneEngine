#include "DUDraw.h"

#include <cmath>

void DUDraw::DrawCircleLinesAngle(int startPosX, int startPosY, float radius, float angle, Color color)
{
    DrawCircleLines(startPosX, startPosY, radius, color);
    DrawLine(startPosX, startPosY, startPosX + radius * cos(angle), startPosY + radius * sin(angle), color);
}

void DUDraw::DrawPolygon(std::vector<Vec2>& vertices, Color color)
{
    // Going through all vertices, drawing lines between them
    for (int i = 0; i < vertices.size(); i++)
    {
        Vec2 start = vertices[i];
        Vec2 end = vertices[(i + 1) % vertices.size()];

        DrawLine(start.x, start.y, end.x, end.y, color);
    }
}

void DUDraw::DrawPolygon(const Vec2& center, std::vector<Vec2>& vertices, Color color)
{
    DrawCircle(center.x, center.y, 2.0f, color);
    DrawPolygon(vertices, color);
}
