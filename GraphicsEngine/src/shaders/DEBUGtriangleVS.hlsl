struct VS_INPUT
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float3 vColor : COLOR;
};

struct VS_OUTPUT
{
    float4 vPosition : SV_POSITION;
    float3 vColor : COLOR;
};


VS_OUTPUT VSMain(VS_INPUT Input)
{
    VS_OUTPUT Output;

    Output.vPosition = float4(Input.vPosition, 1.0f);
    Output.vColor = Input.vColor.xyz;

    return Output;
}
