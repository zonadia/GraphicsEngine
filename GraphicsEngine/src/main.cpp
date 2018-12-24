#include <iostream>
#include "GraphicsWindow.h"
#include "D3DSystem.h"
#include "Shader.h"
#include "Rendering.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int)
{
    mainWindowsWindow = new GraphicsWindow(800, 600, hInstance);
    D3DEngine::initDirectX(mainWindowsWindow);
    
    D3DEngine::loadShaders();

    MSG msg;
    msg.message = WM_NULL;
    PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);
    bool bGotMsg;

    //Main loop
    while (WM_QUIT != msg.message)
    {
        // Process window events.
        // Use PeekMessage() so we can use idle time to render the scene. 
        bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);

        if (bGotMsg)
        {
            // Translate and dispatch the message
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            D3DEngine::renderLoop();
        }

    }


    D3DEngine::cleanupDirectX();
    return 0;
}
