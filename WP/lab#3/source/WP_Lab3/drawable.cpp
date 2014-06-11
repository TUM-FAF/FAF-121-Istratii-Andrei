
#include "drawable.h"
#include "shared_headers.h"


Drawable::Drawable(HBRUSH hbr, HPEN hp)
{
    hBrush = hbr;
    hPen = hp;
}


Drawable::~Drawable()
{
    DeleteObject(hBrush);
    DeleteObject(hPen);
}


void Drawable::SetPenAndBrush(HDC hDC)
{
    SelectObject(hDC, hBrush);
    SelectObject(hDC, hPen);
}


void Drawable::Render(HDC hDC)
{
    SetPenAndBrush(hDC);
    Draw(hDC);
}


StartEndFigure::StartEndFigure(HBRUSH hbr, HPEN hp, int x, int y)
    : Drawable(hbr, hp)
{
    start.x = x;
    start.y = y;

    end.x = x;
    end.y = y;
}


void StartEndFigure::Update(int x, int y, bool constraint)
{
    end.x = x;
    end.y = y;
    if (constraint)
    {
        int dx = start.x - x;
        int dy = start.y - y;
        int delta = min(abs(dx), abs(dy));
        end.x = start.x + delta*(dx<0 ? 1 : -1);
        end.y = start.y + delta*(dy<0 ? 1 : -1);
    }
    else
    {
        end.x = x;
        end.y = y;
    }
}




Rect::Rect(HBRUSH hbr, HPEN hp, int x, int y)
    : StartEndFigure(hbr, hp, x, y) { }


void Rect::Draw(HDC hDC)
{
    Rectangle(hDC, start.x, start.y, end.x, end.y);
}


Elips::Elips(HBRUSH hbr, HPEN hp, int x, int y)
    : StartEndFigure(hbr, hp, x, y) { }

void Elips::Draw(HDC hDC)
{
    Ellipse(hDC, start.x, start.y, end.x, end.y);
}


Bezier::Bezier(HBRUSH hbr, HPEN hp, int x, int y)
    : Drawable(hbr, hp)
{
    POINT p;
    p.x = 100;
    p.y = 100;
    points.push_back(p);
    pointTypes.push_back(PT_MOVETO);

    p.x = 100;
    p.y = 200;
    points.push_back(p);
    pointTypes.push_back(PT_BEZIERTO);

    p.x = 200;
    p.y = 200;
    points.push_back(p);
    pointTypes.push_back(PT_BEZIERTO);

    p.x = 200;
    p.y = 100;
    points.push_back(p);
    pointTypes.push_back(PT_BEZIERTO);

    std::cout << "Bezier drawn\n";
}

void Bezier::Draw(HDC hDC)
{
    PolyDraw(hDC, &points[0], &pointTypes[0], 4);
}

void Bezier::Update(int x, int y, bool constraint)
{

}

