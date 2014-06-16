
#pragma once

class Circle;

class Collision
{
public:
    Collision(Circle* c1_, Circle* c2_);
    float Interpenetration() const;
    void Solve();

    static float Interpenetration(const Circle& c1, const Circle& c2);
    static bool compare(const Collision & col1, const Collision & col2);

protected:
    Circle* c1;
    Circle* c2;
    float interpenetration;
};
