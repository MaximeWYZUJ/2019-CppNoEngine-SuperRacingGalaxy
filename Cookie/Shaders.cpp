#include "pch.h"
#include "Shaders.h"
#include "DeviceD3D11.h"
#include "util.h"
#include "resource.h"
#include "ShaderParams.h"
#include "Texture.h"

namespace Cookie
{
	using namespace std;
	using namespace DirectX;

	Shaders::Shaders(Device* device, const string& shaderName, UINT paramSize, D3D11_INPUT_ELEMENT_DESC* layout, int32_t nbElements, bool hasConstantBuffer, bool hasVertexShader)
		: device{ device }
	{
		string entryPointVS = shaderName + "VS";
		string entryPointPS = shaderName + "PS";

		string t = entryPointVS + ".hlsl";
		wstring shaderFileNameVS = wstring(begin(t), end(t));
		t = entryPointPS + ".hlsl";
		wstring shaderFileNamePS = wstring(begin(t), end(t));

		ID3D11Device* pD3DDevice = static_cast<DeviceD3D11*>(device)->GetD3DDevice();
		
		if(hasVertexShader)
		{
			
			ID3DBlob* pVSBlob = nullptr;
			DXEssayer(D3DCompileFromFile(shaderFileNameVS.c_str(),
				nullptr, nullptr,
				entryPointVS.c_str(),
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
			DXEssayer(pD3DDevice->CreateInputLayout(layout,
				nbElements,
				pVSBlob->GetBufferPointer(),
				pVSBlob->GetBufferSize(),
				&pVertexLayout),
				DXE_CREATIONLAYOUT);
			pVSBlob->Release();
			
		}	

		if(hasConstantBuffer) {
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = paramSize;
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			pD3DDevice->CreateBuffer(&bd, nullptr, &pConstantBuffer);
		}
		
		ID3DBlob* pPSBlob = nullptr;
		DXEssayer(D3DCompileFromFile(shaderFileNamePS.c_str(),
			nullptr, nullptr,
			entryPointPS.c_str(),
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

		// Initialisation des param�tres de sampling de la texture
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
		// Cr�ation de l��tat de sampling
		pD3DDevice->CreateSamplerState(&samplerDesc, &pSamplerState);
	}

	void Shaders::Activate(ShadersParams* sp, vector<ID3D11ShaderResourceView*> textures, bool hasConstantBuffer, bool hasVertexShader) const
	{
		ID3D11DeviceContext* pImmediateContext;
		dynamic_cast<DeviceD3D11*>(device)->GetD3DDevice()->GetImmediateContext(&pImmediateContext);

		if(hasVertexShader)
		{
			pImmediateContext->IASetInputLayout(pVertexLayout);
			pImmediateContext->VSSetShader(pVertexShader, nullptr, 0);
		}
		
		if (hasConstantBuffer)
		{
			pImmediateContext->UpdateSubresource(pConstantBuffer, 0, nullptr, sp, 0, 0);
			pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		}
		
		pImmediateContext->GSSetShader(nullptr, nullptr, 0);
		pImmediateContext->PSSetShader(pPixelShader, nullptr, 0);
		
		if (hasConstantBuffer)
			pImmediateContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		pImmediateContext->PSSetSamplers(0, 1, &pSamplerState);
		
		pImmediateContext->PSSetShaderResources(0, textures.size(), textures.data());
	}
}
