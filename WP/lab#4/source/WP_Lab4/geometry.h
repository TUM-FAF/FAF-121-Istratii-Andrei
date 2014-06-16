
#pragma once

#include <iostream>

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
    Vector2D& operator/=(const float & f);
    Vector2D operator/(const float & f) const;
    float Length() const;
    float SquareLength() const;
    float operator*(const Vector2D & v) const;
    Vector2D operator-() const;
    Vector2D& Normalize();
    float ScalarProjOn(const Vector2D & v);
    Vector2D VectorProjOn(const Vector2D & v);
};

std::ostream & operator<< (std::ostream & out, const Vector2D & v);


float Distance(const Vector2D & v1, const Vector2D & v2);
Vector2D operator*(const float & f, const Vector2D & v);
Vector2D Normalize(const Vector2D & v);

