#pragma once

#include "ExportMacro.h"
#include "Component.h"
#include "Mesh.h"
#include "Device.h"
#include "Engine.h"
#include "Material.h"

namespace SmallEngine
{
	class COOKIE_API MeshRenderer : public Component
	{
	public:
		MeshRenderer(Mesh const& mesh, Material const& material, Device* device);
		void Draw(Engine const& engine);
	private:
		static D3D11_INPUT_ELEMENT_DESC layout[];
		static UINT nbElements;

		Mesh const& mesh;
		Material const& material;
		Device* device;

		Device::BufferPointer pVertexBuffer;
		Device::BufferPointer pIndexBuffer;
		DirectX::XMMATRIX matWorld;
	};
}