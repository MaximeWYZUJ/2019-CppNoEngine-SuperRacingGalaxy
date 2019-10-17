#include "pch.h"
#include "SceneManager.h"
#include "ObjReader.h"
#include "ObjDataToMeshConverter.h"
#include "Device.h"
#include "MeshRenderer.h"
#include "Engine.h"
#include <algorithm>
#include <stack>
using namespace DirectX;

namespace Cookie
{
	using namespace std;

	SceneManager::SceneManager()
	{
		meshes.reserve(1024);
		root.localMatrix = Matrix4x4<>::FromTransform(root.localTransform);
		root.matrix = root.localMatrix;
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
		parent->components.emplace_back(renderer);
		renderer->parent = parent;
		renderer->matrix = &parent->matrix;
		return renderer;
	}

	auto SceneManager::GetRoot() -> SceneNodePtr
	{
		return &root;
	}

	auto SceneManager::AddSceneNode(SceneNodePtr parent) -> SceneNodePtr
	{
		SceneNode* node = nodes.emplace_back(new SceneNode());
		parent->children.push_back(node);
		node->parent = parent;
		// Todo: dirty matrices to require update
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
		// Update matrices
		vector<SceneNode*> c;
		c.reserve(nodes.size());
		stack<SceneNode*, vector<SceneNode*>> nextNodes(move(c));
		for (SceneNode* node : root.children)
		{
			nextNodes.push(node);
		}
		
		while (!nextNodes.empty())
		{
			SceneNode* node = nextNodes.top();
			nextNodes.pop();

			Transform<> const& t = node->localTransform;
			node->localMatrix = Matrix4x4<>::FromTransform(t);
			node->matrix = node->localMatrix * node->parent->matrix;

			for (SceneNode* n : node->children)
			{
				nextNodes.push(n);
			}
		}
		
		// draw meshes
		for(auto& renderer : meshRenderers)
		{
			renderer->Draw(engine);
		}
		return true;
	}
}
