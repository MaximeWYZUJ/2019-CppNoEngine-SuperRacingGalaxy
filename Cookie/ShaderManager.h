#pragma once
#include <string>
#include <map>
#include "Shader.h"
#include "Device.h"

namespace Cookie
{
	class ShaderManager
	{
	public:
		using ShaderType = int;

		ShaderManager(Device* device);

		/// <summary>
		///		Get a vertex shader. Return the same shader instance if called multiple times with the same parameters.
		/// </summary>
		ShaderType GetVertexShader(std::wstring const& fileName);

		/// <summary>
		///		Get a hull shader. Return the same shader instance if called multiple times with the same parameters.
		/// </summary>
		ShaderType GetHullShader(std::wstring const& fileName);

		/// <summary>
		///		Get a domain shader. Return the same shader instance if called multiple times with the same parameters.
		/// </summary>
		ShaderType GetDomainShader(std::wstring const& fileName);

		/// <summary>
		///		Get a pixel shader. Return the same shader instance if called multiple times with the same parameters.
		/// </summary>
		ShaderType GetPixelShader(std::wstring const& fileName);

		void Activate(Shader* shader);

	private:
		Device* device;
		
		std::map<std::wstring, Shader*> shaders;
	};
}
