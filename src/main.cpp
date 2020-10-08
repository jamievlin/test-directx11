#include <windows.h>
#include "MainWindow.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow) {
    MainWindow mainWin;
    mainWin.create();
    mainWin.initialize();
    ShowWindow(mainWin.Window(), nCmdShow);
    return mainWin.run();
}


