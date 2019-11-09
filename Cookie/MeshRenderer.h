#pragma once

#include "ExportMacro.h"
#include "Component.h"
#include "Device.h"

namespace Cookie
{
	class Shaders;
	class Engine;
	class Material;
	class Mesh;
	
	class COOKIE_API MeshRenderer : public Component
	{
	public:
		MeshRenderer(Mesh* mesh, Material* material, Device* device);
		void Draw(Engine const& engine, Shaders const& shader);
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