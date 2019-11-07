#pragma once

#include "DirectXMath.h"
#include "Matrix4x4.h"

namespace Cookie
{
	struct ShadersParams
	{
		Matrix4x4<> matProjViewWorld; // la matrice totale
		Matrix4x4<> matWorld; // matrice de transformation dans le monde
		DirectX::XMVECTOR vLumiere; // la position de la source d’éclairage (Point)
		DirectX::XMVECTOR vCamera; // la position de la caméra
		DirectX::XMVECTOR vAEcl; // la valeur ambiante de l’éclairage
		DirectX::XMVECTOR vAMat; // la valeur ambiante du matériau
		DirectX::XMVECTOR vDEcl; // la valeur diffuse de l’éclairage
		DirectX::XMVECTOR vDMat; // la valeur diffuse du matériau
	};
}