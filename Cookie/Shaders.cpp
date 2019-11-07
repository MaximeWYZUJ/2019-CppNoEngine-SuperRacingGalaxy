#include "pch.h"
#include "Shaders.h"
#include "DeviceD3D11.h"
#include "util.h"
#include "resource.h"
#include "ShaderParams.h"
#include "SommetBloc.h"
#include "Material.h"

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
		DXEssayer(d->CreateInputLayout(CSommetBloc::layout,
			CSommetBloc::numElements,
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
	}

	void Shaders::Activate(const XMMATRIX& matWorld, const XMMATRIX& matViewProj, const Material* mat) const
	{
		static float x = 0.0f;
		static float z = 0.0f;
		static float sign = 1.0f;
		static bool isGrowing = true;

		if (!isGrowing && x < -5.0f)
		{
			sign = 1.0f;
			z += 1.0f;
			isGrowing = true;
		}
		if (isGrowing && x > 10.0f)
		{
			sign = -1.0f;
			z += 1.0f;
			isGrowing = false;
		}

		x += 0.05f * sign;

		
		ShadersParams sp;

		sp.matWorldViewProj = matWorld * matViewProj;
		sp.matWorld = matWorld;
		sp.vLumiere = XMVectorSet(x, 10.0f, z, 1.0f);
		sp.vCamera = XMVectorSet(-5.0f, 5.0f, 1.0f, 1.0f);
		sp.vAEcl = XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
		sp.vAMat = XMVectorSet(mat->ambient.x, mat->ambient.y, mat->ambient.z, mat->ambient.w);
		sp.vDEcl = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
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
	}
}
