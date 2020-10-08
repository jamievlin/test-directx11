#include <Windows.h>
#include <chrono>
#include "common.h"
#include "MainWindow.h"


#pragma region corefuncs

MainWindow::MainWindow() :
    className(MainWindowNames::WIN_CLASS_NAME), windowName(MainWindowNames::WIN_TITLE_NAME),
    winHeight(MainWindowNames::WIN_HEIGHT), winWidth(MainWindowNames::WIN_WIDTH),
    mHwnd(NULL), renderer()
{

}
MainWindow::~MainWindow()
{

}

LRESULT CALLBACK MainWindow::stWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    MainWindow* pThis = nullptr;
    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* createSt = (CREATESTRUCT*)lParam;
        pThis = (MainWindow*)createSt->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
        pThis->mHwnd = hwnd;
    }
    else
    {
        pThis = (MainWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pThis)
    {
        return pThis->handleMsg(uMsg, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

BOOL MainWindow::create()
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = this->stWindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = (PCWSTR)className.c_str();

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, (PCWSTR)className.c_str(), (PCWSTR)windowName.c_str(),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZE,
        CW_USEDEFAULT, CW_USEDEFAULT, winHeight, winWidth,
        NULL, NULL, GetModuleHandle(NULL), this);

    return hwnd ? TRUE : FALSE;
}

void MainWindow::initialize()
{
    renderer.initialize(Window());
}

HWND MainWindow::Window() const
{
    return this->mHwnd;
}
#pragma endregion

LRESULT MainWindow::handleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_WINDOWPOSCHANGING: {
        WINDOWPOS* wPos = (WINDOWPOS*)(lParam);
        wPos->flags |= SWP_NOSIZE;
    }
                             return 0; // do nothing
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(mHwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(mHwnd, &ps);
         }
        return 0;
    }

    return DefWindowProcW(mHwnd, uMsg, wParam, lParam);
}

int MainWindow::run()
{
    MSG msg = {};
    msg.message = WM_NULL;
    PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        auto newTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float, std::milli> duration = newTime - lastTime;
        // update frame here
        renderer.updateLoop(duration.count());
        renderer.drawFrame();
        lastTime = newTime;
    }
    return 0;
}