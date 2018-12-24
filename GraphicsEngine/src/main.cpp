#include <iostream>
#include "GraphicsWindow.h"
#include "D3DSystem.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int)
{
    mainWindowsWindow = new GraphicsWindow(800, 600, hInstance);
    D3D::initDirectX(mainWindowsWindow);

    while(true);


    D3D::cleanupDirectX();
    return 0;
}
