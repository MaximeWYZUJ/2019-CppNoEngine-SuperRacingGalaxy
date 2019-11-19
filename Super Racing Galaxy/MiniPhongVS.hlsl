cbuffer param
{
	float4x4 matProjViewWorld;
	float4x4 matWorld;
	float4 vLumiere;
	float4 vCamera;
	float4 vAEcl;
	float4 vAMat;
	float4 vDEcl;
	float4 vDMat;
}

struct VS_Sortie
{
	float4 Pos : SV_Position;
	float3 Norm : TEXCOORD0;
	float2 texCoord : TEXCOORD1;
	float3 vDirLum : TEXCOORD2;
	float3 vDirCam : TEXCOORD3;
};

Texture2D textureInput;
SamplerState samplerState;

VS_Sortie MiniPhongVS(float4 Pos : POSITION, float3 Normale : NORMAL, float2 texCoord : TEXCOORD)
{
	VS_Sortie sortie = (VS_Sortie)0;
	sortie.Pos = mul(matProjViewWorld, Pos);
	sortie.Norm = mul(matWorld, float4(Normale, 0.0f)).xyz;
	float3 PosWorld = mul(matWorld, Pos).xyz;
	sortie.vDirLum = vLumiere.xyz - PosWorld;
	sortie.vDirCam = vCamera.xyz - PosWorld;

	sortie.texCoord = texCoord;

	return sortie;
}