cbuffer param
{
	float4x4 matWorldViewProj; // la matrice totale
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

VS_Sortie MiniPhongVS(float4 Pos : POSITION, float3 Normale : NORMAL, float2 texCoord : TEXCOORD)
{
	VS_Sortie sortie = (VS_Sortie)0;
	sortie.Pos = mul(matWorldViewProj, Pos);
	sortie.Norm = mul(matWorld, float4(Normale, 0.0f)).xyz;
	float3 PosWorld = mul(matWorld, Pos).xyz;
	sortie.vDirLum = vLumiere.xyz - PosWorld;
	sortie.vDirCam = vCamera.xyz - PosWorld;

	sortie.texCoord = texCoord;

	return sortie;
}