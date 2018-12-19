#pragma once
#include <d3d11.h>
#include <dxgi1_3.h>
#include <vector>

class GraphicsWindow;

namespace D3D
{
    void initDirectX(GraphicsWindow *window);
    void cleanupDirectX();

    //****************************************
    //Variables used in directx
    //****************************************
    ID3D11Texture2D *backBuffer;
    ID3D11Texture2D *depthStencil;
    D3D11_TEXTURE2D_DESC bbDesc;

    ID3D11Device *DXdevice;
    ID3D11DeviceContext *DXcontext;
    IDXGIDevice3 *dxgiDevice;

    //Used for swap chain creation
    IDXGIAdapter *adapter;
    IDXGIFactory *factory;

    IDXGISwapChain *swapChain;

    ID3D11RenderTargetView *renderTargetView;
    ID3D11DepthStencilView *depthStencilView;
    ID3D11DepthStencilState *depthStencilState;
    ID3D11RasterizerState *rasterizerState;

    D3D11_VIEWPORT viewport;
}
