#pragma once

#include "ExportMacro.h"
#include "Component.h"
#include "Mesh.h"
#include "Device.h"
#include "Material.h"

namespace Cookie
{
	class Engine;
	
	class COOKIE_API MeshRenderer : public Component
	{
	public:
		MeshRenderer(Mesh* mesh, Material* material, Device* device);
		void Draw(Engine const& engine);
	private:
		static D3D11_INPUT_ELEMENT_DESC layout[];
		static UINT nbElements;

		Mesh* mesh;
		Material* material;
		Device* device;

		Device::BufferPointer pVertexBuffer;
		Device::BufferPointer pIndexBuffer;
	};
}