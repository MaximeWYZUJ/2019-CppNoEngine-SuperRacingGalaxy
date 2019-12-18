#include "Deferred.hlsli"

Texture2D shaderTexture : register(t0);
SamplerState SampleTypeWrap : register(s0);

struct PixelOutputType
{
    float4 color : SV_Target0;
    float4 normal : SV_Target1;
};

PixelOutputType DeferredPS(PixelInputType input) : SV_TARGET
{
    PixelOutputType output;
    output.color = shaderTexture.Sample(SampleTypeWrap, input.tex);
    output.normal = float4(input.normal, 1.0f);

    return output;
}