#pragma once
#include "ShaderParams.h"

namespace Cookie
{
	struct SpriteParams : ShadersParams
	{
		Matrix4x4<> matWVP; // la matrice totale
	};	
}