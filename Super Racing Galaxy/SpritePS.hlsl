cbuffer param
{
	float4x4 matWVP; // La matrice de travail
}

struct VS_Sortie
{
	float4 Pos : SV_Position;
	float2 coordTex : TEXCOORD0;
};

Texture2D textureInput;
SamplerState samplerState;

float4 SpritePS(VS_Sortie vs) : SV_Target
{
	float4 couleurTexture;
	couleurTexture = textureInput.Sample(samplerState, vs.coordTex);

	return couleurTexture;
}