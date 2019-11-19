#include "pch.h"
#include "Shaders.h"
#include "DeviceD3D11.h"
#include "util.h"
#include "resource.h"
#include "ShaderParams.h"
#include "Layout.h"
#include "Material.h"
#include "Texture.h"

namespace Cookie
{
	using namespace std;
	using namespace DirectX;

	Shaders::Shaders(Device* device)
		: device{ device }
	{
		ID3D11Device* pD3DDevice = static_cast<DeviceD3D11*>(device)->GetD3DDevice();
		ID3DBlob* pVSBlob = nullptr;
		DXEssayer(D3DCompileFromFile(L"MiniPhongVS.hlsl",
			nullptr, nullptr,
			"MiniPhongVS",
			"vs_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS,
			0,
			&pVSBlob, nullptr), DXE_FICHIER_VS);

		DXEssayer(pD3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(),
			nullptr,
			&pVertexShader),
			DXE_CREATION_VS);

		pVertexLayout = nullptr;
		ID3D11Device* d = static_cast<DeviceD3D11*>(device)->GetD3DDevice();
		DXEssayer(d->CreateInputLayout(VertexData::layout,
			VertexData::nbElements,
			pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(),
			&pVertexLayout),
			DXE_CREATIONLAYOUT);
		pVSBlob->Release();

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ShadersParams);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		pD3DDevice->CreateBuffer(&bd, nullptr, &pConstantBuffer);

		ID3DBlob* pPSBlob = nullptr;
		DXEssayer(D3DCompileFromFile(L"MiniPhongPS.hlsl",
			nullptr, nullptr,
			"MiniPhongPS",
			"ps_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS,
			0,
			&pPSBlob,
			nullptr), DXE_FICHIER_PS);
		DXEssayer(pD3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(),
			pPSBlob->GetBufferSize(),
			nullptr,
			&pPixelShader),
			DXE_CREATION_PS);
		pPSBlob->Release();

		// Initialisation des paramètres de sampling de la texture
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		// Création de l’état de sampling
		pD3DDevice->CreateSamplerState(&samplerDesc, &pSamplerState);
	}

	void Shaders::Activate(const Matrix4x4<>& matWorld, const Matrix4x4<>& matViewProj, Vector3<> const& camPos, const Material* mat) const
	{
		static float x = 0.0f;
		static float z = 0.0f;
		static float sign = 1.0f;
		static bool isGrowing = true;

		if (!isGrowing && x < -10.0f)
		{
			sign = 1.0f;
			isGrowing = true;
		}
		if (isGrowing && x > 10.0f)
		{
			sign = -1.0f;
			isGrowing = false;
		}

		x += 0.05f * sign;
		z += 0.01f;

		
		ShadersParams sp;

		sp.matProjViewWorld = matViewProj * matWorld;
		sp.matWorld = matWorld;
		sp.vLumiere = XMVectorSet(x, 25.0f, z, 0.0f);
		sp.vCamera = XMVectorSet(camPos.x, camPos.y, camPos.z, 1.0f);
		sp.vAEcl = XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);
		sp.vAMat = XMVectorSet(mat->ambient.x, mat->ambient.y, mat->ambient.z, mat->ambient.w);
		sp.vDEcl = XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);
		sp.vDMat = XMVectorSet(mat->diffuse.x, mat->diffuse.y, mat->diffuse.z, mat->diffuse.w);
		
		ID3D11DeviceContext* pImmediateContext;
		dynamic_cast<DeviceD3D11*>(device)->GetD3DDevice()->GetImmediateContext(&pImmediateContext);

		pImmediateContext->IASetInputLayout(pVertexLayout);
		pImmediateContext->VSSetShader(pVertexShader, nullptr, 0);
		pImmediateContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &sp, 0, 0);
		pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pImmediateContext->GSSetShader(nullptr, nullptr, 0);
		pImmediateContext->PSSetShader(pPixelShader, nullptr, 0);
		pImmediateContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		pImmediateContext->PSSetSamplers(0, 1, &pSamplerState);
		auto* texture = mat->texture->GetD3DTexture();
		pImmediateContext->PSSetShaderResources(0, 1, &texture);
	}
}
