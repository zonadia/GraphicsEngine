struct PS_INPUT
{
    float4 vPosition : SV_POSITION;
    float3 vColor : COLOR;
};

float4 PSMain(PS_INPUT Input) : SV_TARGET
{
    return float4(Input.vColor, 1.0f);
}