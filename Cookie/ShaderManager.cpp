#include "pch.h"
#include "ShaderManager.h"
#include "resource.h"
#include "Util.h"
#include "DeviceD3D11.h"
#include "Layout.h"
#include <locale>
#include <codecvt>
#include <string>

namespace Cookie
{
	using namespace std;
	
	ShaderManager::ShaderManager(Device* device)
		: device(device)
	{
	}

	auto ShaderManager::GetVertexShader(std::wstring const& fileName) -> ShaderType
	{
		/*wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
		string entryPoint = converter.to_bytes(fileName.substr(0, fileName.find('.') + 1));*/

		//ID3D11Device* d3d11Device = static_cast<DeviceD3D11*>(device)->GetD3DDevice();
		//ID3DBlob* pVSBlob = nullptr;
		//DXEssayer(D3DCompileFromFile(fileName.c_str(),
		//	nullptr, nullptr,
		//	entryPoint.c_str(),
		//	"vs_5_0",
		//	D3DCOMPILE_ENABLE_STRICTNESS,
		//	0,
		//	&pVSBlob, nullptr), DXE_FICHIER_VS);

		//DXEssayer(d3d11Device->CreateVertexShader(pVSBlob->GetBufferPointer(),
		//	pVSBlob->GetBufferSize(),
		//	nullptr,
		//	&pVertexShader),
		//	DXE_CREATION_VS);

		//pVertexLayout = nullptr;
		//ID3D11Device* d = static_cast<DeviceD3D11*>(device)->GetD3DDevice();
		//DXEssayer(d->CreateInputLayout(VertexData::layout,
		//	nbElements,
		//	pVSBlob->GetBufferPointer(),
		//	pVSBlob->GetBufferSize(),
		//	&pVertexLayout),
		//	DXE_CREATIONLAYOUT);
		//pVSBlob->Release();

		//D3D11_BUFFER_DESC bd;
		//ZeroMemory(&bd, sizeof(bd));
		//bd.Usage = D3D11_USAGE_DEFAULT;
		//bd.ByteWidth = paramSize;
		//bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		//bd.CPUAccessFlags = 0;
		//d3d11Device->CreateBuffer(&bd, nullptr, &pConstantBuffer);

		//ID3DBlob* pPSBlob = nullptr;
		//DXEssayer(D3DCompileFromFile((shaderName + L"PS.hlsl").c_str(),
		//	nullptr, nullptr,
		//	entrypointPS,
		//	"ps_5_0",
		//	D3DCOMPILE_ENABLE_STRICTNESS,
		//	0,
		//	&pPSBlob,
		//	nullptr), DXE_FICHIER_PS);
		//DXEssayer(d3d11Device->CreatePixelShader(pPSBlob->GetBufferPointer(),
		//	pPSBlob->GetBufferSize(),
		//	nullptr,
		//	&pPixelShader),
		//	DXE_CREATION_PS);
		//pPSBlob->Release();

		//// Initialisation des paramètres de sampling de la texture
		//D3D11_SAMPLER_DESC samplerDesc;
		//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		//samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		//samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		//samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		//samplerDesc.MipLODBias = 0.0f;
		//samplerDesc.MaxAnisotropy = 1;
		//samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		//samplerDesc.BorderColor[0] = 0;
		//samplerDesc.BorderColor[1] = 0;
		//samplerDesc.BorderColor[2] = 0;
		//samplerDesc.BorderColor[3] = 0;
		//samplerDesc.MinLOD = 0;
		//samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		//// Création de l’état de sampling
		//d3d11Device->CreateSamplerState(&samplerDesc, &pSamplerState);
		return ShaderType();
	}

	auto ShaderManager::GetHullShader(std::wstring const& fileName) -> ShaderType
	{
		return ShaderType();
	}

	auto ShaderManager::GetDomainShader(std::wstring const& fileName) -> ShaderType
	{
		return ShaderType();
	}

	auto ShaderManager::GetPixelShader(std::wstring const& fileName) -> ShaderType
	{
		return ShaderType();
	}

}