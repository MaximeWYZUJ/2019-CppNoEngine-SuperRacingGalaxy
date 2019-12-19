cbuffer param {
	float distance; // distance radial Blur
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

float4 RadialBlurPS(VS_Sortie vs) : SV_Target
{
	float4 couleur;
	float4 ct;
	float2 tc = vs.CoordTex;
	float d, dx, dy;
	
	couleur = 0;
	float x = tc.x * 2 - 1.0;
	float y = tc.y * 2 - 1.0;
	dx = sqrt(x * x); // Distance du centre
	dy = sqrt(y * y); // Distance du centre 
	
	dx = x * (distance * dx); // Le dégradé (blur) est en fonction de la 
	dy = y * (distance * dy); // distance du centre et de la variable distance. 
	
	x = x - (dx * 10); // Vous pouvez jouer avec le nombre d’itérations 
	y = y - (dy * 10);
	
	tc.x = (x + 1.0) / 2.0;
	tc.y = (y + 1.0) / 2.0;
	
	for (int i = 0; i < 10; i++) // Vous pouvez jouer avec le nombre d’itérations
	{
		ct = textureEntree.Sample(SampleState, tc);
		couleur = couleur * 0.6 + ct * 0.4; // Vous pouvez « jouer » avec les % 
		
		x = x + dx;
		y = y + dy;
	
		tc.x = (x + 1.0) / 2.0;
		tc.y = (y + 1.0) / 2.0;
	
	}
	
	return couleur;
}