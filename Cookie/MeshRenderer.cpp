#include "pch.h"
#include "MeshRenderer.h"
#include "DeviceD3D11.h"
#include "SommetBloc.h"
#include "Engine.h"
#include "ShaderParams.h"
#include "Material.h"

namespace Cookie
{
	using namespace std;
	using namespace DirectX;

	D3D11_INPUT_ELEMENT_DESC MeshRenderer::layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT MeshRenderer::nbElements = ARRAYSIZE(MeshRenderer::layout);

	MeshRenderer::MeshRenderer(Mesh* mesh, Material* material, Device* device)
		: mesh{ mesh }, material{ material }, device{ device }
	{
		BufferDescription desc;
		
		{
			auto& vertices = mesh->GetVertices();
			auto& normals = mesh->GetNormals();
			unique_ptr<CSommetBloc[]> sommets = make_unique<CSommetBloc[]>(vertices.size());

			for (auto i = 0; i < vertices.size(); ++i)
			{
				sommets[i] = CSommetBloc(
					XMFLOAT3(vertices[i].x, vertices[i].z, vertices[i].y),
					*reinterpret_cast<XMFLOAT3 const*>(&normals[i]),
					XMFLOAT2(0.0f, 0.0f));
			}

			BufferDescription desc
			{
				.size = static_cast<uint32_t>(sizeof(CSommetBloc) * vertices.size()),
				.buffer = BufferFlag::Vertex,
				.GpuMemory = GpuMemoryType::ReadOnly,
				.CpuAccess = CpuAccessFlag::Nothing
			};

			pVertexBuffer = device->CreateBuffer(desc, sommets.get());
		}

		{
			auto& triangleSources = mesh->GetTriangles();
			unique_ptr<uint32_t[]> triangles = make_unique<uint32_t[]>(triangleSources.size() * 3);
			for (auto i = 0; i < triangleSources.size(); ++i)
			{
				triangles[i * 3] = triangleSources[i].A;
				triangles[i * 3 + 1] = triangleSources[i].B;
				triangles[i * 3 + 2] = triangleSources[i].C;
			}

			desc.size = triangleSources.size() * 3 * 4;
			desc.buffer = BufferFlag::Index;
			desc.GpuMemory = GpuMemoryType::ReadOnly;
			desc.CpuAccess = CpuAccessFlag::Nothing;

			pIndexBuffer = device->CreateBuffer(desc, triangles.get());
		}
	}

	void MeshRenderer::Draw(Engine const& engine) // oof... is it normal that each renderer needs a ref to the engine?
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
		
		ID3D11DeviceContext* pImmediateContext;
		dynamic_cast<DeviceD3D11*>(device)->GetD3DDevice()->GetImmediateContext(&pImmediateContext);
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		constexpr UINT stride = sizeof(CSommetBloc);
		constexpr UINT offset = 0;
		pImmediateContext->IASetVertexBuffers(0, 1, reinterpret_cast<ID3D11Buffer* const*>(&pVertexBuffer), &stride, &offset);
		pImmediateContext->IASetIndexBuffer(reinterpret_cast<ID3D11Buffer*>(pIndexBuffer), DXGI_FORMAT_R32_UINT, 0);

		Matrix4x4<> const& projViewWorld = engine.GetMatViewProj() * *matrix;
		
		ShadersParams sp;
		sp.matProjViewWorld = projViewWorld;
		sp.matWorld = *matrix;
		sp.vLumiere = XMVectorSet(x, 10.0f, z, 1.0f);
		sp.vCamera = XMVectorSet(-5.0f, 5.0f, 1.0f, 1.0f);
		sp.vAEcl = XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
		sp.vAMat = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
		sp.vDEcl = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
		sp.vDMat = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

		material->Activate(sp);
		
		pImmediateContext->DrawIndexed(mesh->GetTriangles().size() * 3, 0, 0);
	}
}
