#pragma once
#include <Windows.h>
#include <d3dcommon.h>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

namespace D3DEngine
{

struct DefaultVertex
{
    //Position
    float x, y, z;
    //Normal
    float nx, ny, nz;
    //Color
    float r, g, b;
};

//A combination of a vertex and pixel shader
struct ShaderProgram
{
    ID3D11VertexShader *vertexShader;
    ID3D11PixelShader *pixelShader;

    ID3D11InputLayout *inputLayout;
};

HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob);

int loadShaders();


extern ShaderProgram mainShaderProgram;
}
