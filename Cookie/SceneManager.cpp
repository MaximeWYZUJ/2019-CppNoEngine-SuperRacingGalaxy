#include "pch.h"
#include <algorithm>
#include "SceneManager.h"
#include "ObjReader.h"
#include "ObjDataToMeshConverter.h"
#include "Device.h"
#include "MeshRenderer.h"
#include "Engine.h"
#include <algorithm>
#include <stack>
#include "Util.h"

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
		node->localTransform.SetDirty();
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

		int64_t lastStackSize = numeric_limits<int64_t>::max();
		while (!nextNodes.empty())
		{
			SceneNode* node = nextNodes.top();
			nextNodes.pop();

			if (node->localTransform.IsDirty())
			{
				node->localTransform.ResetDirty();
				lastStackSize = min(static_cast<int64_t>(nextNodes.size()), lastStackSize);
				
				Transform<> const& t = node->localTransform;
				node->localMatrix = Matrix4x4<>::FromTransform(t);
				UpdateNodeAndStackChildren(node, StackInserter(nextNodes));
			}
			else if (nextNodes.size() >= lastStackSize)
			{
				UpdateNodeAndStackChildren(node, StackInserter(nextNodes));
			}
		}
		
		// draw meshes
		for(auto& renderer : meshRenderers)
		{
			renderer->Draw(engine);
		}
		return true;
	}

	void SceneManager::UpdateNodeAndStackChildren(SceneNode* node, StackInsertIterator<std::stack<SceneNode*, std::vector<SceneNode*>>> insertIt)
	{
		node->matrix = node->parent->matrix * node->localMatrix;

		for (SceneNode* n : node->children)
		{
			insertIt = n;
		}
	}
}
