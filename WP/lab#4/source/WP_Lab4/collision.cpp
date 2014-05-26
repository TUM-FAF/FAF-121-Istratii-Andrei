
#include "collision.h"
#include "shared_headers.h"


Collision::Collision(Circle* c1_, Circle* c2_)
{
    c1 = c1_;
    c2 = c2_;
    interpenetration = Interpenetration(*c1, *c2);
}


float Collision::Interpenetration() const
{
    return interpenetration;
}


void Collision::Solve()
{
    Vector2D collisionVector = c1->position - c2->position;

    float m1 = c1->radius * c1->radius;
    float m2 = c2->radius * c2->radius;
    std::cout << "Masses: " << m1 << ", " << m2 << std::endl;

    std::cout << "Velocities: " << (c1->velocity) << ", " << (c2->velocity) << std::endl;

    Vector2D preCollPart1 = c1->velocity.VectorProjOn(collisionVector);
    Vector2D preCollPart2 = c2->velocity.VectorProjOn(collisionVector);

    std::cout << "PreColl: " << preCollPart1 << ", " << preCollPart2 << std::endl;

    Vector2D tangPart1 = c1->velocity - preCollPart1;
    Vector2D tangPart2 = c2->velocity - preCollPart2;

    std::cout << "TanPart: " << tangPart1 << ", " << preCollPart2 << std::endl;

    Vector2D postCollPart1 = (m1*preCollPart1 + m2*(2*preCollPart2 - preCollPart1))/(m1 + m2);
    Vector2D postCollPart2 = postCollPart1 + preCollPart1 - preCollPart2;

    c1->velocity = (tangPart1 + postCollPart1)*0.95;
    c2->velocity = (tangPart2 + postCollPart2)*0.95;

    c1->position += Normalize(collisionVector) * interpenetration;
    c2->position -= Normalize(collisionVector) * interpenetration;
}


float Collision::Interpenetration(const Circle& c1, const Circle& c2)
{
    return c1.radius + c2.radius - Distance(c1.position, c2.position);
}


bool Collision::compare(const Collision & col1, const Collision & col2)
{
    return col1.Interpenetration() < col2.Interpenetration();
}

