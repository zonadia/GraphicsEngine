#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <fstream>

#include "GraphicsWindow.h"

#define CLASSNAME L"Allie's Graphics Engine"
#define WINDOWNAME L"Allie's Totes Awesomes Graphicx Engine~ ^.^"

GraphicsWindow *mainWindowsWindow;

void RedirectIOToConsole(FILE *consoleFP);


GraphicsWindow::GraphicsWindow(int windowWidth, int windowHeight, HINSTANCE hInstance) : width(windowWidth), height(windowHeight), consoleFP(0)
{
    RedirectIOToConsole(consoleFP);
    int error = InitWindowClass(hInstance);
    if(error)
    {
        std::cout << "Error initializing window!\n";
    }
}

int GraphicsWindow::InitWindowClass(HINSTANCE hInstance)
{
    mainWindowsWindow = this;
    //Set up global variables
    hInst = &hInstance;

    if (hInstance == NULL)
        hInstance = (HINSTANCE)GetModuleHandle(NULL);

    HICON hIcon = NULL;
    WCHAR szExePath[MAX_PATH];
    GetModuleFileName(NULL, szExePath, MAX_PATH);

    // If the icon is NULL, then use the first one found in the exe
    if (hIcon == NULL)
        hIcon = ExtractIcon(hInstance, szExePath, 0);

    // Set up WNDCLASS
    wndClass.style = CS_DBLCLKS;
    wndClass.lpfnWndProc = WindowProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = hIcon;
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = CLASSNAME;

    //Register the WNDCLASS
    if (!RegisterClass(&wndClass))
    {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_CLASS_ALREADY_EXISTS)
            return HRESULT_FROM_WIN32(dwError);
    }

    //Create the window
    int x = CW_USEDEFAULT;
    int y = CW_USEDEFAULT;

    // Resizable 800x800 window
    int wDefaultWidth = width = 800;
    int wDefaultHeight = height = 800;

    RECT windowRectangle;

    SetRect(&windowRectangle, 0, 0, wDefaultWidth, wDefaultHeight);
    AdjustWindowRect(&windowRectangle, WS_OVERLAPPED, false);

    // Create the window for our viewport.
    windowHandle = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW, wndClass.lpszClassName, WINDOWNAME, WS_OVERLAPPEDWINDOW, x, y, (windowRectangle.right - windowRectangle.left), (windowRectangle.bottom - windowRectangle.top),
        0, NULL, hInstance, 0);


    if (windowHandle == NULL)
    {
        DWORD dwError = GetLastError();
        return HRESULT_FROM_WIN32(dwError);
    }

    ShowWindow(windowHandle, true);

    return 0;
}

void RedirectIOToConsole(FILE *consoleFP)
{
    AllocConsole();
    freopen_s(&consoleFP, "CONOUT$", "wb", stdout);
}

//WinProc function for dealing with windows messages
LRESULT CALLBACK GraphicsWindow::WindowProc(_In_ HWND   hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    /*
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
        return true;*/

    switch (uMsg)
    {
    case WM_SIZE:
    {
        std::cout << "Window resized!\n";
        mainWindowsWindow->width = LOWORD(lParam);
        mainWindowsWindow->height = HIWORD(lParam);
        return 0;
    }

    case WM_CLOSE:
    {
        std::cout << "Requested to close window\n";
        HMENU hMenu;
        hMenu = GetMenu(hwnd);
        if (hMenu != NULL)
        {
            DestroyMenu(hMenu);
        }
        DestroyWindow(hwnd);
        UnregisterClass(
            CLASSNAME,
            *(mainWindowsWindow->hInst)
        );
        return 0;
    }

    case WM_DESTROY:
        std::cout << "Window getting obliterated by Trump's space force o.O\n";
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
