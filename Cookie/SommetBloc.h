#pragma once

namespace SmallEngine
{
	class CSommetBloc
	{
	public:
		CSommetBloc() = default;
		CSommetBloc(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& normal, DirectX::XMFLOAT2 const& coordTex);
	public:
		static UINT numElements;
		static D3D11_INPUT_ELEMENT_DESC layout[];
	private:
		DirectX::XMFLOAT3 m_Position;
		DirectX::XMFLOAT3 m_Normal;
		DirectX::XMFLOAT2 m_TexCoord;
	};
}