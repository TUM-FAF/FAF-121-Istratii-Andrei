
#pragma once

#include "mouse.h"
#include "viewport_window.h"


class Game
{
public:
    virtual void Init();
    virtual void SceneInit();
    virtual void Run();

    virtual void GetInput(int message, WPARAM wParam, LPARAM lParam);
    virtual void Update();
    virtual void Render(double inFrame);

private:
    ViewportWindow viewport;
    Mouse mouse;

    int lineAngle;
};

