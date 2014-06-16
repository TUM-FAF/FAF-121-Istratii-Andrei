
#include "geometry.h"
#include "shared_headers.h"

Vector2D::Vector2D()
{
    x = 0.0f;
    y = 0.0f;
}


Vector2D::Vector2D(float x_, float y_)
{
    x = x_;
    y = y_;
}


Vector2D::Vector2D(const Vector2D & v)
{
    x = v.x;
    y = v.y;
}


Vector2D& Vector2D::operator+=(const Vector2D & v)
{
    x += v.x;
    y += v.y;
    return *this;
}


Vector2D Vector2D::operator+(const Vector2D & v) const
{
    return Vector2D(x + v.x, y + v.y);
}


Vector2D& Vector2D::operator-=(const Vector2D & v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}


Vector2D Vector2D::operator-(const Vector2D & v) const
{
    return Vector2D(x - v.x, y - v.y);
}


Vector2D& Vector2D::operator*=(const float & f)
{
    x *= f;
    y *= f;
    return *this;
}


Vector2D Vector2D::operator*(const float & f) const
{
    return Vector2D(x * f, y * f);
}


Vector2D& Vector2D::operator/=(const float & f)
{
    x /= f;
    y /= f;
    return *this;
}


Vector2D Vector2D::operator/(const float & f) const
{
    return Vector2D(x / f, y / f);
}


Vector2D operator*(const float & f, const Vector2D & v)
{
    return v*f;
}


float Distance(const Vector2D & v1, const Vector2D & v2)
{
    return (v2 - v1).Length();
}


float Vector2D::Length() const
{
    return sqrt(x*x + y*y);
}


float Vector2D::SquareLength() const
{
    return x*x + y*y;
}


float Vector2D::operator*(const Vector2D & v) const
{
    return x*v.x + y*v.y;
}


Vector2D Vector2D::operator- () const
{
    return Vector2D(-x, -y);
}


Vector2D& Vector2D::Normalize()
{
    (*this) /= Length();
    return *this;
}


Vector2D Normalize(const Vector2D & v)
{
    return v / v.Length();
}


float Vector2D::ScalarProjOn(const Vector2D & v)
{
    return (*this) * v / v.Length();
}


Vector2D Vector2D::VectorProjOn(const Vector2D & v)
{
    return ((*this) * v / v.SquareLength()) * v;
}


std::ostream & operator<< (std::ostream & out, const Vector2D & v)
{
    out << "(" << v.x << ", " << v.y << ")";
    return out;
}

