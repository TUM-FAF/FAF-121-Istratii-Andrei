
#pragma once

#include "geometry.h"
#include <windows.h>




class Circle
{
    friend class Collision;
public:
    Circle();
    Circle(int x, int y, int r);
    void CheckBorderCollision(int width, int height);
    void UpdatePosition();
    void ApplyGravity(float g);
    void SetVelocity(float x, float y);
    void Draw(HDC hDC, float inFrame);

private:
    int radius;
    Vector2D position;
    Vector2D velocity;

};
