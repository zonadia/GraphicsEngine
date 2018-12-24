#include "GraphicsWindow.h"
#include "Shader.h"
#include "D3DSystem.h"
#include <d3dcompiler.h>
#include <d3d11.h>
#include <dxgi1_3.h>
#include <iostream>

namespace D3DEngine
{
    ShaderProgram mainShaderProgram;

HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob)
{
    if (!srcFile || !entryPoint || !profile || !blob)
        return E_INVALIDARG;

    *blob = nullptr;

    const D3D_SHADER_MACRO defines[] =
    {
        "EXAMPLE_DEFINE", "1",
        NULL, NULL
    };

    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint, profile,
        0, 0, &shaderBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            std::cout << "Shader Compile Error!\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
            errorBlob->Release();
        }

        if (shaderBlob)
            shaderBlob->Release();

        return hr;
    }

    *blob = shaderBlob;

    return hr;
}

int loadDefaultLayoutShader(LPCWSTR vShaderName, LPCWSTR pShaderName, ID3D11VertexShader **vShader, ID3D11PixelShader **pShader, ID3D11InputLayout **inLayout)
{
    // Compile vertex shader
    ID3DBlob *vsBlob = nullptr;
    HRESULT hr = CompileShader(vShaderName, "VSMain", "vs_4_0", &vsBlob);
    if (FAILED(hr))
    {
        std::cout << "Failed compiling vertex shader!" << std::endl;
        return -1;
    }

    std::cout << "Compiled vertex shader!\n";

    D3D11_INPUT_ELEMENT_DESC iaDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
        0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
        0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

        { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT,
        0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = DXdevice->CreateInputLayout(iaDesc, 3, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), inLayout);

    if (FAILED(hr))
    {
        std::cout << "Failed to create vertex shader input layout!\n";
        return -1;
    }

    // Compile pixel shader
    ID3DBlob *psBlob = nullptr;
    hr = CompileShader(pShaderName, "PSMain", "ps_4_0", &psBlob);
    if (FAILED(hr))
    {
        vsBlob->Release();
        std::cout << "Failed compiling pixel shader!" << std::endl;
        return -1;
    }

    std::cout << "Compiled pixel shader!\n";


    //Create shaders
    hr = DXdevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, vShader);
    if (FAILED(hr))
    {
        std::cout << "Failed to create vertex shader!\n";
    }
    hr = DXdevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, pShader);
    if (FAILED(hr))
    {
        std::cout << "Failed to create pixel shader!\n";
    }

    // Clean up shader blobs
    vsBlob->Release();
    psBlob->Release();

    return 0;
}

//Return -1 on fail
int loadShaders()
{
    //Triangle test
    loadDefaultLayoutShader(L"../src/shaders/DEBUGtriangleVS.hlsl", L"../src/shaders/DEBUGtrianglePS.hlsl", &mainShaderProgram.vertexShader, &mainShaderProgram.pixelShader, &mainShaderProgram.inputLayout);

    return 0;
}

}
