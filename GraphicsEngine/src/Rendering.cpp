#include "Rendering.h"
#include "D3DSystem.h"
#include "Shader.h"
#include <iostream>

namespace D3DEngine
{

    void DEBUGrenderTriangle()
    {
        //Set shaders as active
        DXcontext->VSSetShader(mainShaderProgram.vertexShader, 0, 0);
        DXcontext->PSSetShader(mainShaderProgram.pixelShader, 0, 0);



        //Set up vertex buffers

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

        HRESULT hr = DXdevice->CreateBuffer(&bd, NULL, &vertBuffer);
        if (FAILED(hr)) {
            std::cout << "Failed to create buffer for rendering!: " << GetLastError() << std::endl;
            return;
        }
        //Fill the buffer
        D3D11_MAPPED_SUBRESOURCE ms;
        DXcontext->Map(vertBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        memcpy(ms.pData, TriVertices, sizeof(TriVertices));
        DXcontext->Unmap(vertBuffer, NULL);



        //Do the actual rendering
        //Set up active things the GPU needs to know
        UINT stride = sizeof(DefaultVertex);
        UINT offset = 0;
        DXcontext->IASetVertexBuffers(0, 1, &vertBuffer, &stride, &offset);
        //Also set the input layout
        DXcontext->IASetInputLayout(mainShaderProgram.inputLayout);

        DXcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        DXcontext->Draw(3, 0);

        vertBuffer->Release();
    }

    //Render the scene
    void renderLoop()
    {
        DXcontext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

        //Clear the back buffer
        float clearCol[4] = {1.0f, 0.6f, 0.8f, 1.0f};
        DXcontext->ClearRenderTargetView(renderTargetView, clearCol);
        DXcontext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        //Render the frame! ^.^
        DEBUGrenderTriangle();



        // Present the frame to the screen.
        swapChain->Present(0, 0);
    }
}
