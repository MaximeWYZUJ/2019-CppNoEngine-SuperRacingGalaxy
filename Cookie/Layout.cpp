#include "pch.h"
#include "Layout.h"

namespace Cookie
{
	D3D11_INPUT_ELEMENT_DESC VertexData::layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	
	VertexData::VertexData(Vector3<> const& position_, Vector3<> const& normal_, Vector2<> const& texCoord_)
	{
		position = position_;
		normal = normal_;
		texCoord = texCoord_;
	}
}