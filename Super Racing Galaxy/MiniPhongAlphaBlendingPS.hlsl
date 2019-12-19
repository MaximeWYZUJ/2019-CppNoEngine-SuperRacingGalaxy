cbuffer param
{
	float4x4 matProjViewWorld; // la matrice totale
	float4x4 matWorld; // matrice de transformation dans le monde
	float4 vLumiere; // la position de la source d’éclairage (Point)
	float4 vCamera; // la position de la caméra
	float4 vAEcl; // la valeur ambiante de l’éclairage
	float4 vAMat; // la valeur ambiante du matériau
	float4 vDEcl; // la valeur diffuse de l’éclairage
	float4 vDMat; // la valeur diffuse du matériau
}

struct VS_Sortie
{
	float4 Pos : SV_Position;
	float3 Norm : TEXCOORD0;
	float2 texCoord : TEXCOORD1;
	float3 vDirLum : TEXCOORD2;
	float3 vDirCam : TEXCOORD3;
};

Texture2D textureInput[3];
SamplerState samplerState;

float4 MiniPhongAlphaBlendingPS(VS_Sortie vs) : SV_Target0
{
	float3 couleur;
	// Normaliser les paramètres
	float3 N = normalize(vs.Norm);
	float3 L = normalize(vs.vDirLum);
	float3 V = normalize(vs.vDirCam);
	// Valeur de la composante diffuse
	float3 diff = saturate(dot(N, L));
	// R = 2 * (N.L) * N – L
	float3 R = normalize(2 * diff.xyz * N - L);

	float3 textureColor1 = textureInput[0].Sample(samplerState, vs.texCoord * 10.0f).rgb;
	float3 textureColor2 = textureInput[1].Sample(samplerState, vs.texCoord * 10.0f).rgb;
	float3 textureAlpha = textureInput[2].Sample(samplerState, vs.texCoord).rgb;
	float3 textureColor = textureColor1 * (1.0f - textureAlpha) + textureColor2 * textureAlpha;

	textureColor = saturate(textureColor);

	// I = A + D * N.L + (R.V)n
	couleur = textureColor * vAEcl.rgb * vAMat.rgb +
		textureColor * vDEcl.rgb * vDMat.rgb * diff;

	return float4(couleur, 1.0f);
}
