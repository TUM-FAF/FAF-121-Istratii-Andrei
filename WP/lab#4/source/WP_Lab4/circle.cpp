
#include "circle.h"
#include "shared_headers.h"

Circle::Circle()
{
    radius = 0;
}

Circle::Circle(int x, int y, int r)
{
    radius = r;
    position.x = float(x);
    position.y = float(y);
}


void Circle::UpdatePosition()
{
    position += velocity;
}


void Circle::SetVelocity(float x, float y)
{
    velocity = Vector2D(x, y);
}


void Circle::CheckBorderCollision(int width, int height)
{
    if (position.x - radius < 0)
    {
        position.x = radius;
        velocity.x *= -1;
    }

    if (position.y - radius < 0)
    {
        position.y = radius;
        velocity.y *= -1;
    }

    if (position.x + radius > width)
    {
        position.x = width - radius;
        velocity.x *= -1;
    }

    if (position.y + radius > height)
    {
        position.y = height - radius;
        velocity.y *= -1;
    }
}


void Circle::Draw(HDC hDC, float inFrame)
{
    Vector2D origin(position - Vector2D(radius, radius));
    float r2 = 2 * radius;
    Ellipse(hDC, origin.x, origin.y, origin.x + r2, origin.y + r2);
}
