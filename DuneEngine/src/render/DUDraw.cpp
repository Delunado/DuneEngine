#include "DUDraw.h"

#include <cmath>

void DUDraw::DrawCircleLinesAngle(int startPosX, int startPosY, float radius, float angle, Color color)
{
    DrawCircleLines(startPosX, startPosY, radius, color);
    DrawLine(startPosX, startPosY, startPosX + radius * cos(angle), startPosY + radius * sin(angle), color);
}
