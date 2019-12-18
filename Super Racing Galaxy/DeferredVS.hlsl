#include "Deferred.hlsli"

PixelInputType DeferredVS(VertexInputType input)
{
    PixelInputType output;
    output.pos = mul(projViewWorldMatrix, input.pos);
    output.tex = input.tex;

    // Calculate the normal vector against the world matrix only.
    output.normal = mul((float3x3)worldMatrix, input.normal);
    output.normal = normalize(output.normal);

    return output;
}