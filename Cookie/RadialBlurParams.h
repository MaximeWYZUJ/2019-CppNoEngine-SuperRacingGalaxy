#pragma once
#include "ShaderParams.h"

namespace Cookie
{
	struct RadialBlurParams : ShadersParams
	{
		float distance;
		int remplissage1;
		int remplissage2;
		int remplissage3;
	};
}

