
#pragma once

#include <windows.h>

enum Tool
{
    SELECT, ELLIPSE, RECTANGLE, PEN, ERASER
};

struct DrawingOptions
{
    Tool tool;
    COLORREF fillColor;
    bool noFill;
    COLORREF strokeColor;
    bool noStroke;
    unsigned int strokeWidth;
    bool shiftConstraint;

    DrawingOptions()
    {
        tool = SELECT;
        fillColor = RGB(255, 255, 255);
        noFill = true;
        strokeColor = RGB(0, 0, 0);
        noStroke = false;
        strokeWidth = 1;
        shiftConstraint = false;
    }
};