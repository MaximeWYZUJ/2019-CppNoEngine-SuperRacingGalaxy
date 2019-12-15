#pragma once
#include "Matrix4x4.h"

namespace Cookie
{
	struct COOKIE_API Sprite
	{
		ID3D11ShaderResourceView* pTextureD3D;
		
		Matrix4x4<> matPosDim;
		Sprite() : pTextureD3D{ nullptr } {}
	};
}

