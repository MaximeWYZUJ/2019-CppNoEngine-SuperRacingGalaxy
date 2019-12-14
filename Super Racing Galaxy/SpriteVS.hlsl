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

VS_Sortie SpriteVS(float4 pos : POSITION, float2 coordTex : TEXCOORD )
{
	VS_Sortie sortie = (VS_Sortie)0;

	sortie.Pos = mul(matWVP, pos);

	//Coordonnées d'application de texture
	sortie.coordTex = coordTex;

	return sortie;
}