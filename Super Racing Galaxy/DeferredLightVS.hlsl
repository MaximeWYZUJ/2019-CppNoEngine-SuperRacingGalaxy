cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix projViewWorldMatrix;
};

struct VertexInputType
{
    float4 pos : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

PixelInputType DeferredLightVS(VertexInputType input)
{
    PixelInputType output;
    output.pos = mul(projViewWorldMatrix, input.pos);
    output.tex = input.tex;

    return output;
}