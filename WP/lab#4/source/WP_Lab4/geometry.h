
#pragma once

struct Vector2D
{
    float x;
    float y;

    Vector2D();
    Vector2D(float x, float y);
    Vector2D(const Vector2D & v);
    Vector2D& Vector2D::operator+=(const Vector2D & v);
    Vector2D operator+(const Vector2D & v) const;
    Vector2D& Vector2D::operator-=(const Vector2D & v);
    Vector2D operator-(const Vector2D & v) const;
    Vector2D& operator*=(const float & f);
    Vector2D operator*(const float & f) const;
    float Length() const;
    float SquareLength() const;
};

float Distance(const Vector2D & v1, const Vector2D & v2);
Vector2D operator*(const float & f, const Vector2D & v);

