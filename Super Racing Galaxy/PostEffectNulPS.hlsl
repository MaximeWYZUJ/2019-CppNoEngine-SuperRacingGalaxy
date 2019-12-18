struct VS_Sortie
{
	float4 Pos : SV_POSITION;
	float2 CoordTex : TEXCOORD0;
};

Texture2D textureEntree; // la texture 
SamplerState SampleState; // l��tat de sampling 

float4 PostEffectPS(VS_Sortie vs) : SV_Target
{ 
	float4 couleur;
	couleur = textureEntree.Sample(SampleState, vs.CoordTex);
	return couleur; 
}