#include "pch.h"
#include "SceneManager.h"
#include "ObjReader.h"
#include "ObjDataToMeshConverter.h"
#include "Device.h"
#include "MeshRenderer.h"
#include "Engine.h"
#include <algorithm>

namespace Cookie
{
	using namespace std;

	SceneManager::SceneManager()
	{
		meshes.reserve(1024);
	}

	void SceneManager::SetDevice(Device* device)
	{
		this->device = device;
	}

	Mesh* SceneManager::GetMesh(string const& filePath)
	{
		ObjData res = ObjReader::Read(filePath);
		MeshPtr meshRes = ObjDataToMeshConverter::Convert(res);
		Mesh* mesh = meshes.emplace_back(meshRes);

		return mesh;
	}

	MeshRenderer* SceneManager::AddMeshRenderer(Mesh* mesh, Material* mat, SceneNode* parent)
	{
		MeshRenderer* renderer = meshRenderers.emplace_back(new MeshRenderer(mesh, mat, device));
		parent->Components.emplace_back(renderer);
		renderer->Parent = parent;
		renderer->Transform = &parent->Transform;
		return renderer;
	}

	auto SceneManager::GetRoot() -> SceneNodePtr
	{
		return &root;
	}

	auto SceneManager::AddSceneNode(SceneNodePtr parent) -> SceneNodePtr
	{
		SceneNode* node = nodes.emplace_back(new SceneNode());
		parent->Children.push_back(node);
		// Todo: update matrices
		return node;
	}

	void SceneManager::RemoveSceneNode(SceneNodePtr node)
	{
		for (auto it = begin(nodes); it != end(nodes); ++it)
		{
			if (*it == node)
			{
				delete *it;
				nodes.erase(it);
				break;
			}
		}
	}

	bool SceneManager::DrawAll(Engine const& engine)
	{
		for(auto& renderer : meshRenderers)
		{
			renderer->Draw(engine);
		}
		return true;
	}
}
