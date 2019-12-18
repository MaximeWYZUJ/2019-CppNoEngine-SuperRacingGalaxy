struct VS_Sortie 
{ 
	float4 Pos : SV_POSITION; 
	float2 CoordTex : TEXCOORD0; 
}; 

Texture2D textureEntree; // la texture 
SamplerState SampleState; // l��tat de sampling 
				
VS_Sortie PostEffectNulVS(float4 Pos : POSITION, float2 CoordTex : TEXCOORD0 )
{ 
	VS_Sortie sortie = (VS_Sortie)0;
	sortie.Pos = Pos;
	sortie.CoordTex = CoordTex;
	return sortie; 
}