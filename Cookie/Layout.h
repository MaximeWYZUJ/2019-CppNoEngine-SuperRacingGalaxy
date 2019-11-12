#pragma once

#include "Vector3.h"
#include "Vector2.h"

namespace Cookie
{
	template<class ... Parents>
	struct Layout : Parents...
	{
		static const int32_t nbElements = sizeof...(Parents);
	};

	struct LayoutPosition
	{
		Vector3<> position;
	};

	struct LayoutNormal
	{
		Vector3<> normal;
	};

	struct LayoutTexCoord
	{
		Vector2<> texCoord;
	};

	struct VertexData : Layout<LayoutPosition, LayoutNormal, LayoutTexCoord>
	{
		static D3D11_INPUT_ELEMENT_DESC layout[];

		VertexData() = default;
		VertexData(Vector3<> const& position_, Vector3<> const& normal_, Vector2<> const& texCoord_);
	};
}
