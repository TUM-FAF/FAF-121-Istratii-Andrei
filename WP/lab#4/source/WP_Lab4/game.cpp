
#include "game.h"
#include "shared_headers.h"


Game::Game()
{

}


Game::~Game()
{
    std::list<Circle*>::iterator itr;
    for (itr = balls.begin(); itr != balls.end(); ++itr)
    {
        delete (*itr);
        (*itr) = NULL;
    }
}


void Game::Init()
{
    INITCOMMONCONTROLSEX ctls;
    ctls.dwSize = sizeof(INITCOMMONCONTROLSEX);
    ctls.dwICC = ICC_BAR_CLASSES;

    InitCommonControlsEx(&ctls);

    CreateDeveloperConsole();

    RECT rct;

    rct.left = 0;
    rct.top = 0;
    rct.right = 800;
    rct.bottom = 600;

    AdjustWindowRectEx(&rct, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL, TRUE, WS_EX_OVERLAPPEDWINDOW);

    bool res = viewport.CreateEx(WS_EX_OVERLAPPEDWINDOW,
        L"WP Lab 4",
        L"WP Lab 4",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0,
        rct.right - rct.left, rct.bottom - rct.top,
        NULL, NULL,
        GetModuleHandle(NULL));

    if (!res)
    {
        throw Error("Could not create the window!");
    }

    viewport.Show(SW_SHOW);
    viewport.Update();
}


void Game::SceneInit()
{
    //balls.push_back(new Circle(100, 100, 50));
    //balls.front()->SetVelocity(5, 10);
}


void Game::Run()
{

    MSG msg;
    bool running = true;

    double lag = 0.0;
    const double MS_PER_UPDATE = 16.66666;
    Timer timer;

    while (running)
    {
        lag += timer.GetTime();
        timer.Reset();

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) { running = false; }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            GetInput(msg.message, msg.wParam, msg.lParam);

        }

        while (lag >= MS_PER_UPDATE)
        {
            if (needUpdate)
            {
                Update();
                needUpdate = true;
            }
            lag -= MS_PER_UPDATE;
        }

        Render(lag/MS_PER_UPDATE);
    }
}


void Game::GetInput(int message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_MOUSEMOVE:
        {
            POINT m;
            m.x = GET_X_LPARAM(lParam);
            m.y = GET_Y_LPARAM(lParam);

            mouse.Update(m.x, m.y);
        }
        break;
    case WM_LBUTTONDOWN:
        OnLeftButtonDown();
        break;

    case WM_MOUSEWHEEL:
        OnRightButtonDown();
        break;

    default:
        return;
    }
}


void Game::OnLeftButtonDown()
{
    balls.push_back(new Circle(mouse.X(), mouse.Y(), 30));
}


void Game::OnRightButtonDown()
{
    needUpdate = true;
}


void Game::Update()
{
    std::list<Circle*>::iterator ball1;
    for (ball1 = balls.begin(); ball1 != balls.end(); ++ball1)
    {
        (*ball1)->ApplyGravity(1);
        (*ball1)->UpdatePosition();
        (*ball1)->CheckBorderCollision(viewport.GetWidth(), viewport.GetHeight());

        std::list<Circle*>::iterator ball2;
        for (ball2 = std::next(ball1); ball2 != balls.end(); ++ball2)
        {
            if (Collision::Interpenetration(**ball1, **ball2) >= 0)
            {
                collisions.push_back(Collision(*ball1, *ball2));
            }
        }
    }

    std::sort(collisions.begin(), collisions.end(), Collision::compare);

    std::vector<Collision>::iterator col;
    for (col = collisions.begin(); col != collisions.end(); ++col)
    {
        col->Solve();
    }
    collisions.clear();
}


void Game::Render(double inFrame)
{
    float pi = 3.14159265f;

    Backbuffer* bb = viewport.GetBackbuffer();
    bb->FillWithColor(RGB(200, 200, 200));
    HDC hDC = bb->GetDC();


    std::list<Circle*>::iterator itr;
    for (itr = balls.begin(); itr != balls.end(); ++itr)
    {
        (*itr)->Draw(hDC, inFrame);
    }


    InvalidateRect(viewport.GetHWND(), NULL, FALSE);
}

