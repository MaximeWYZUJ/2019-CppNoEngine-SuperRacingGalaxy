#include "pch.h"
#include "MeshRenderer.h"
#include "DeviceD3D11.h"
#include "Layout.h"
#include "Engine.h"
#include "Material.h"
#include "Shaders.h"
#include "ShaderParams.h"
#include "MiniPhongParams.h"

namespace Cookie
{
	using namespace std;
	using namespace DirectX;

	MeshRenderer::MeshRenderer(Mesh* mesh, Material* material, Device* device)
		: mesh{ mesh }, material{ material }, device{ device }
	{
		BufferDescription desc;
		
		{
			auto& vertices = mesh->GetVertices();
			auto& textureCoords = mesh->GetTextureCoords();
			auto& normals = mesh->GetNormals();
			unique_ptr<VertexData[]> gpuVertices = make_unique<VertexData[]>(vertices.size());

			for (int i = 0; i < vertices.size(); ++i)
			{
				gpuVertices[i] = VertexData(
					{ vertices[i].x, vertices[i].y, vertices[i].z },
					{ normals[i].x, normals[i].y, normals[i].z },
					textureCoords[i]);
			}

			BufferDescription desc
			{
				.size = static_cast<uint32_t>(sizeof(VertexData) * vertices.size()),
				.buffer = BufferFlag::Vertex,
				.GpuMemory = GpuMemoryType::ReadOnly,
				.CpuAccess = CpuAccessFlag::Nothing
			};

			pVertexBuffer = device->CreateBuffer(desc, gpuVertices.get());
		}

		{
			auto& triangleSources = mesh->GetTrianglesDx();
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

	void MeshRenderer::Draw(Matrix4x4<> const& projView, Vector3<> camPos, Shaders const& shader)
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


		MiniPhongParams* sp = new MiniPhongParams;

		sp->matProjViewWorld = projView * *matrix;
		sp->matWorld = *matrix;
		sp->vLumiere = XMVectorSet(x, 25.0f, z, 0.0f);
		sp->vCamera = XMVectorSet(camPos.x, camPos.y, camPos.z, 1.0f);
		sp->vAEcl = XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);
		sp->vAMat = XMVectorSet(material->ambient.x, material->ambient.y, material->ambient.z, material->ambient.w);
		sp->vDEcl = XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);
		sp->vDMat = XMVectorSet(material->diffuse.x, material->diffuse.y, material->diffuse.z, material->diffuse.w);
		
		ID3D11DeviceContext* pImmediateContext;
		dynamic_cast<DeviceD3D11*>(device)->GetD3DDevice()->GetImmediateContext(&pImmediateContext);
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		constexpr UINT stride = sizeof(VertexData);
		constexpr UINT offset = 0;
		pImmediateContext->IASetVertexBuffers(0, 1, reinterpret_cast<ID3D11Buffer* const*>(&pVertexBuffer), &stride, &offset);
		pImmediateContext->IASetIndexBuffer(reinterpret_cast<ID3D11Buffer*>(pIndexBuffer.p), DXGI_FORMAT_R32_UINT, 0);

		ID3D11ShaderResourceView* texture = material->textures[0]->GetD3DTexture();
		UINT size = sizeof(MiniPhongParams);
		shader.Activate(sp, texture);
		
		pImmediateContext->DrawIndexed(mesh->GetTrianglesDx().size() * 3, 0, 0);
	}
}
