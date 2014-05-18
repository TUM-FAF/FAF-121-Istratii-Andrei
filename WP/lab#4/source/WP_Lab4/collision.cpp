
#include "collision.h"
#include "shared_headers.h"


Collision::Collision(Circle& c1_, Circle& c2_)
{
    c1 = c1_;
    c2 = c2_;
}


float Collision::Interpenetration()
{
    return c1.radius + c2.radius - Distance(c1.position, c2.position);
}


void Collision::Solve()
{

}
