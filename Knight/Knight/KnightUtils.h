#pragma once

#include "Knight.h"

struct BoundingRect {
    Vector2 min;
    Vector2 max;
};

extern Rectangle& CenterRectangle(Rectangle& r, int width, int height);
extern float DegreesToRadians(float degrees);
extern BoundingRect Get2DBoundingRectOfCube(const Vector3& cubePosition, float cubeSize, const Camera3D& camera);