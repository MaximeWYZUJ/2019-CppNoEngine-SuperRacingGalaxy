struct VS_Sortie
{
	float4 Pos : SV_POSITION;
	float2 CoordTex : TEXCOORD0;
};

Texture2D textureEntree; // la texture 
SamplerState SampleState; // l’état de sampling 

float4 PostEffectNulPS(VS_Sortie vs) : SV_Target
{ 
	float4 couleur;
	couleur = textureEntree.Sample(SampleState, vs.CoordTex);
	return couleur; 
}