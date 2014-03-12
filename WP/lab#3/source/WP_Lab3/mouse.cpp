
#include "mouse.h"


void Mouse::Update(int x_, int y_)
{
    dx = x_ - x;
    dy = y_ - y;
    x = x_;
    y = y_;
}