#pragma once



class Mouse
{
public:
    Mouse() : x(0), y(0), dx(0), dy(0) { }

    void Update(int x_, int y_);
    int X() { return x; }
    int Y() { return y; }
    int DX() { return dx; }
    int DY() { return dy; }

private:
    int x;
    int y;
    int dx;
    int dy;
};