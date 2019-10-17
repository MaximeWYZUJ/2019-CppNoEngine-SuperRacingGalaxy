#pragma once

#include "DirectXMath.h"

namespace Cookie
{
	template<class ... Parents>
	struct Layout : Parents...
	{
		static const int32_t nbElements = sizeof...(Parents);
	};

	struct LayoutPosition
	{
		DirectX::XMFLOAT3 position;
	};

	struct LayoutNormal
	{
		DirectX::XMFLOAT3 normal;
	};

	struct LayoutTexCoord
	{
		DirectX::XMFLOAT2 texCoord;
	};

	struct Layout3d : Layout<LayoutPosition, LayoutNormal, LayoutTexCoord>
	{
	};
}