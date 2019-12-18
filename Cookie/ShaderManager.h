#pragma once
#include <string>
#include <map>
#include "Device.h"
#include "D3D11VertexShader.h"
#include "ShaderParams.h"
#include "D3D11PixelShader.h"

namespace Cookie
{
	/// <summary>
	///		Load/Unload shaders.
	/// </summary>
	/// <remarks>
	///		This class may be an interface to switch between OpenGL/Vulkan/Direct3D implementations.
	/// </remarks>
	class ShaderManager
	{
	public:
		enum class ShaderType : uint16_t
		{
			Vertex,
			Hull,
			Domain,
			Pixel
		};
		
		struct ShaderId
		{
			ShaderType type;
			uint16_t id;
		};

		struct MatrixBuffer : ShadersParams
		{
			Matrix4x4<> worldMatrix;
			Matrix4x4<> projViewWorldMatrix;
		};

		ShaderManager(Device* device);

		/// <summary>
		///		Get a vertex/pixel shader combination. Return the same shader instance if called multiple times with the same parameters.
		/// </summary>
		D3D11VPShader* GetVPShader(std::string const& shaderName);

		/// <summary>
		///		Activate a shader.
		/// </summary>
		void Activate(ShaderId shaderId);

	private:
		Device* device;

		std::vector<D3D11VPShader> vpShaders;
		std::map<std::string, int> nameToShaders;
	};
}
