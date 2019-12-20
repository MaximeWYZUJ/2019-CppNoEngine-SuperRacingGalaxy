#include "pch.h"
#include "DeferredShader.h"
#include "resource.h"
#include <string>
#include "DeviceD3D11.h"
#include "Util.h"
#include "Device.h"
#include "Layout.h"

namespace Cookie
{
	using namespace std;
	
	DeferredShader::DeferredShader(Device* device)
		: device(device)
	{
		/*string vEntryPoint = "DeferredVS";
		wstring vFileName = ToWide(vEntryPoint + ".hlsl");
		string pEntryPoint = "DeferredPS";
		wstring pFileName = ToWide(pEntryPoint + ".hlsl");*/

		/*ID3D11Device* d = static_cast<DeviceD3D11*>(device)->GetD3DDevice();
		ID3DBlob* pVSBlob = nullptr;
		DXEssayer(D3DCompileFromFile(vFileName.c_str(),
			nullptr, nullptr,
			vEntryPoint.c_str(),
			"vs_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS,
			0,
			&pVSBlob, nullptr), DXE_FICHIER_VS);

		DXEssayer(d->CreateVertexShader(pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(),
			nullptr,
			&vertexShader),
			DXE_CREATION_VS);

		DXEssayer(d->CreateInputLayout(VertexData::layout,
			VertexData::nbElements,
			pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(),
			&inputLayout),
			DXE_CREATIONLAYOUT);
		pVSBlob->Release();

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof bd);
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof DeferredShaderParams;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		d->CreateBuffer(&bd, nullptr, &matrixBuffer);

		ID3DBlob* pPSBlob = nullptr;
		DXEssayer(D3DCompileFromFile(pFileName.c_str(),
			nullptr, nullptr,
			pEntryPoint.c_str(),
			"ps_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS,
			0,
			&pPSBlob,
			nullptr), DXE_FICHIER_PS);

		DXEssayer(d->CreatePixelShader(pPSBlob->GetBufferPointer(),
			pPSBlob->GetBufferSize(),
			nullptr,
			&pixelShader),
			DXE_CREATION_PS);
		pPSBlob->Release();

		D3D11_SAMPLER_DESC desc;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.BorderColor[0] = 0;
		desc.BorderColor[1] = 0;
		desc.BorderColor[2] = 0;
		desc.BorderColor[3] = 0;
		desc.MinLOD = 0;
		desc.MaxLOD = D3D11_FLOAT32_MAX;
		d->CreateSamplerState(&desc, &samplerState);*/
	}
	
	void DeferredShader::Activate(DeferredShaderParams const& p)
	{
		/*ID3D11DeviceContext* c;
		auto d = dynamic_cast<DeviceD3D11*>(device)->GetD3DDevice();
		d->GetImmediateContext(&c);

		c->IASetInputLayout(inputLayout);
		c->VSSetShader(vertexShader, nullptr, 0);

		c->UpdateSubresource(matrixBuffer, 0, nullptr, &p, 0, 0);
		c->VSSetConstantBuffers(0, 1, &matrixBuffer);

		c->GSSetShader(nullptr, nullptr, 0);
		c->PSSetShader(pixelShader, nullptr, 0);
		c->PSSetConstantBuffers(0, 1, &matrixBuffer);
		c->PSSetSamplers(0, 1, &samplerState);*/
		//c->PSSetShaderResources(0, 1, &texture);
	}
}
