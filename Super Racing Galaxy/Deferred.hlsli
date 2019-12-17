cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix projViewWorldMatrix;
};

struct VertexInputType
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};