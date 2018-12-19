#pragma once
#include <Windows.h>

class GraphicsWindow
{
public:
    GraphicsWindow(int windowWidth, int windowHeight, HINSTANCE hInstance);

    HINSTANCE *hInst;
    WNDCLASS wndClass;
    HWND windowHandle;
    int width, height;

private:
    static LRESULT CALLBACK WindowProc(_In_ HWND   hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
    FILE *consoleFP;
    int InitWindowClass(HINSTANCE hInstance);
    //*****************************************************************************************
    //Have a queue of winProc messages that the graphics application (or whateva~) can handle
    //*****************************************************************************************
};

extern GraphicsWindow *mainWindowsWindow;
