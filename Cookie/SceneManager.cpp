#include "pch.h"
#include "SceneManager.h"
#include "ObjReader.h"
#include "ObjDataToMeshConverter.h"
#include "Device.h"
#include "DeviceD3D11.h"
#include "MeshRenderer.h"
#include "Engine.h"

namespace SmallEngine
{
	using namespace std;

	SceneManager::SceneManager()
	{
		// Todo: Need a multi-segment vector (no copy when growing)
		sceneNodes.reserve(1024);
		meshes.reserve(1024);
	}

	void SceneManager::SetDevice(Device* device)
	{
		this->device = device;
	}

	Mesh* SceneManager::GetMesh(string const& filePath)
	{
		ObjData res = ObjReader::Read(filePath);
		Mesh meshRes = ObjDataToMeshConverter::Convert(res);
		Mesh* mesh = &meshes.emplace_back(meshRes);

		return mesh;
	}

	SceneNode* SceneManager::GetRoot()
	{
		return &root;
	}

	SceneNode* SceneManager::CreateSceneNode()
	{
		return &sceneNodes.emplace_back(SceneNode{});
	}

	bool SceneManager::AnimateAll()
	{
		return true;
	}

	bool SceneManager::DrawAll(Engine const& engine)
	{
		for (auto& child : root.GetChildren())
		{
			vector<Component*> components = child->GetComponents();
			MeshRenderer* renderer = static_cast<MeshRenderer*>(components[0]);
			renderer->Draw(engine);
		}
		return true;
	}
}