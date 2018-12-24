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
    extern ID3D11Texture2D *backBuffer;
    extern ID3D11Texture2D *depthStencil;
    extern D3D11_TEXTURE2D_DESC bbDesc;

    extern ID3D11Device *DXdevice;
    extern ID3D11DeviceContext *DXcontext;
    extern IDXGIDevice3 *dxgiDevice;

    //Used for swap chain creation
    extern IDXGIAdapter *adapter;
    extern IDXGIFactory *factory;

    extern IDXGISwapChain *swapChain;

    extern ID3D11RenderTargetView *renderTargetView;
    extern ID3D11DepthStencilView *depthStencilView;
    extern ID3D11DepthStencilState *depthStencilState;
    extern ID3D11RasterizerState *rasterizerState;

    extern D3D11_VIEWPORT viewport;
}
