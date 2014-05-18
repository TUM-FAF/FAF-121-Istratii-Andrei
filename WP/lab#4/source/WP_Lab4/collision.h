
#pragma once

#include "circle.h"

class Collision
{
public:
    Collision(Circle& c1_, Circle& c2_);
    float Interpenetration();
    void Solve();

private:
    Circle c1;
    Circle c2;
};
