#pragma once
#include <Windows.h>
#include <string>
#include "Renderer.h"

class MainWindow {
public:
    static LRESULT CALLBACK stWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    MainWindow();
    ~MainWindow();
    BOOL create();
    HWND Window() const;
    void initialize();
    int run();

protected:
    HWND mHwnd;
    virtual LRESULT handleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
    std::wstring className;
    std::wstring windowName;
    UINT winHeight;
    UINT winWidth;

    Renderer renderer;
};