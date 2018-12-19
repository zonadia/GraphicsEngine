#include "D3DSystem.h"
#include "GraphicsWindow.h"
#include <iostream>

//Forward declarations
void D3D::createRasterizerState();
void D3D::createDepthStencilState();
void D3D::initDirectX(HWND windowHandle);
void D3D::DEBUGrenderTriangle();

namespace D3D
{
    void initDirectX(GraphicsWindow *window)
    {

    }
    void cleanupDirectX()
    {

    }



    //*****************************************************
    //DirectX Initialization helper functions
    //*****************************************************
    void createRasterizerState()
    {
        D3D11_RASTERIZER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));

        desc.AntialiasedLineEnable = false;
        desc.CullMode = D3D11_CULL_BACK;
        desc.DepthBias = 0;
        desc.DepthBiasClamp = 0.0f;
        desc.DepthClipEnable = true;
        desc.FillMode = D3D11_FILL_SOLID;
        desc.FrontCounterClockwise = true; // RH Coordinate System
        desc.MultisampleEnable = false;
        desc.ScissorEnable = false;
        desc.SlopeScaledDepthBias = 0.0f;

        HRESULT hr = DXdevice->CreateRasterizerState(&desc, &rasterizerState);
        if (FAILED(hr))std::cout << "Failed to create rasterizer state! " << GetLastError() << std::endl;

        DXcontext->RSSetState(rasterizerState);
    }

    void createDepthStencilState()
    {
        D3D11_DEPTH_STENCIL_DESC desc;
        ZeroMemory(&desc, sizeof(desc));

        desc.DepthEnable = true;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        desc.DepthFunc = D3D11_COMPARISON_LESS;

        desc.StencilEnable = true;
        desc.StencilReadMask = 0xFF;
        desc.StencilWriteMask = 0xFF;

        // Stencil operations if pixel is front-facing.
        desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        // Stencil operations if pixel is back-facing.
        desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        HRESULT hr = DXdevice->CreateDepthStencilState(&desc, &depthStencilState);
        if (FAILED(hr))std::cout << "Failed to create depth stencil state!\n";

        DXcontext->OMSetDepthStencilState(depthStencilState, 1);
    }

    void initDirectX(HWND windowHandle)
    {
        D3D_FEATURE_LEVEL levels[] = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_11_1
        };

        // This flag adds support for surfaces with a color-channel ordering different
        // from the API default. It is required for compatibility with Direct2D.
        UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;


        // Create the Direct3D 11 API device object and a corresponding context.

        HRESULT hr = D3D11CreateDevice(
            nullptr,                    // Specify nullptr to use the default adapter.
            D3D_DRIVER_TYPE_HARDWARE,   // Create a device using the hardware graphics driver.
            0,                          // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
            deviceFlags,                // Set debug and Direct2D compatibility flags.
            levels,                     // List of feature levels this app can support.
            ARRAYSIZE(levels),          // Size of the list above.
            D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
            &DXdevice,          // Returns the Direct3D device created.
            NULL,                       // Returns feature level of device created.
            &DXcontext          // Returns the device immediate context.
        );

        if (FAILED(hr))
        {
            // Handle device interface creation failure if it occurs.
            // For example, reduce the feature level requirement, or fail over 
            // to WARP rendering.
            std::cout << "Failed to create Direct3D Device! :'(\n";
        }

        //Create device and swap chain
        DXGI_SWAP_CHAIN_DESC desc;
        ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
        desc.Windowed = TRUE; // Sets the initial state of full-screen mode to not full screen
        desc.BufferCount = 2;
        desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.SampleDesc.Count = 1;      //multisampling setting
        desc.SampleDesc.Quality = 0;    //vendor-specific flag
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
        desc.OutputWindow = windowHandle;

        // Create the DXGI device object to use in other factories, such as Direct2D.
        //DXdevice.As(&dxgiDevice);
        hr = DXdevice->QueryInterface(__uuidof(IDXGIDevice3), (void **)&dxgiDevice);

        // Create swap chain.
        hr = dxgiDevice->GetAdapter(&adapter);

        if (SUCCEEDED(hr))
        {
            adapter->GetParent(IID_PPV_ARGS(&factory));

            hr = factory->CreateSwapChain(DXdevice, &desc, &swapChain);
        }

        //Create a render target
        hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

        hr = DXdevice->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);

        backBuffer->GetDesc(&bbDesc);

        //Create a depth stencil
        createDepthStencilState();
        CD3D11_TEXTURE2D_DESC depthStencilDesc(
            DXGI_FORMAT_D24_UNORM_S8_UINT,
            static_cast<UINT> (bbDesc.Width),
            static_cast<UINT> (bbDesc.Height),
            1, // This depth stencil view has only one texture.
            1, // Use a single mipmap level.
            D3D11_BIND_DEPTH_STENCIL
        );

        hr = DXdevice->CreateTexture2D(
            &depthStencilDesc,
            nullptr,
            &depthStencil);
        if (FAILED(hr)) {
            std::cout << GetLastError() << std::endl;
            return;
        }
        CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);

        hr = DXdevice->CreateDepthStencilView(
            depthStencil,
            &depthStencilViewDesc,
            &depthStencilView);
        if (FAILED(hr)) {
            std::cout << GetLastError() << std::endl;
            return;
        }
        createRasterizerState();

        //Create a viewport
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
        viewport.Height = (float)bbDesc.Height;
        viewport.Width = (float)bbDesc.Width;
        viewport.MinDepth = 0;
        viewport.MaxDepth = 1;

        DXcontext->RSSetViewports(1, &viewport);

        loadShaders();
    }

    void DEBUGrenderTriangle()
    {
        //Set shaders as active
        DXcontext->VSSetShader(mainShaderProgram.vertexShader.Get(), 0, 0);
        DXcontext->PSSetShader(mainShaderProgram.pixelShader.Get(), 0, 0);



        //Set up vertex buffers
        /*DefaultVertex TriVertices[] = {{0.0f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f},
                                       { -0.45f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f },
                                       { -0.45f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f }};*/

        DefaultVertex TriVertices[] = { {-0.50f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f},
                                       { 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f },
                                       { 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f } };

        //Put buffer into mesh later
        //Create buffer
        ID3D11Buffer *vertBuffer;
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(DefaultVertex) * 3;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        HRESULT hr = DXData::DXdevice->CreateBuffer(&bd, NULL, &vertBuffer);
        if (FAILED(hr)) {
            std::cout << GetLastError() << std::endl;
            return;
        }
        //Fill the buffer
        D3D11_MAPPED_SUBRESOURCE ms;
        DXData::DXcontext->Map(vertBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        memcpy(ms.pData, TriVertices, sizeof(TriVertices));
        DXData::DXcontext->Unmap(vertBuffer, NULL);



        //Do the actual rendering
        //Set up active things the GPU needs to know
        UINT stride = sizeof(DefaultVertex);
        UINT offset = 0;
        DXData::DXcontext->IASetVertexBuffers(0, 1, &vertBuffer, &stride, &offset);
        //Also set the input layout
        DXData::DXcontext->IASetInputLayout(DXData::mainShaderProgram.vsLayout);

        DXData::DXcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        DXData::DXcontext->Draw(3, 0);

        vertBuffer->Release();
    }


}




