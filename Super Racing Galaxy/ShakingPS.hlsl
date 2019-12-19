cbuffer param {
	int temps; // temps passé
	int remplissage1;
	int remplissage2;
	int remplissage3;
};

struct VS_Sortie
{
	float4 Pos : SV_POSITION;
	float2 CoordTex : TEXCOORD0;
};

Texture2D textureEntree; // la texture 
SamplerState SampleState; // l’état de sampling 

float4 ShakingPS(VS_Sortie vs) : SV_Target
{
	float4 couleur;
	float2 tc = vs.CoordTex;

		if (temps < 3) {
			tc.x = tc.x + temps * 0.0008;
			tc.y = tc.y + temps * 0.0004;
		}
		else {
			tc.x = tc.x  + 0.0024 - temps * 0.0008;
			tc.y = tc.y + 0.0012 - temps * 0.0004;
		}

		if (tc.x < 1.0 && tc.y < 1.0 && tc.x > 0.0 && tc.y > 0.0) {
			couleur = textureEntree.Sample(SampleState, tc);
		}

		else {
			couleur = (0.0, 0.0, 0.0, 0.0);
		}

	return couleur;
}