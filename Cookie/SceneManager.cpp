#include "pch.h"
#include <algorithm>
#include <stack>
#include "SceneManager.h"
#include "ObjReader.h"
#include "ObjDataToMeshConverter.h"
#include "Device.h"
#include "MeshRenderer.h"
#include "Engine.h"
#include "Util.h"
#include "AlgorithmShortcuts.h"
#include "PhysicsBoxComponent.h"

using namespace DirectX;

namespace Cookie
{
	using namespace std;

	SceneManager::SceneManager(Device* device) : device { device }, shaders{ device }
	{
		meshes.reserve(1024);
		root.localMatrix = Matrix4x4<>::FromTransform(root.localTransform);
		root.matrix = root.localMatrix;
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
		
		parent->components.push_back(renderer);
		renderer->parent = parent;
		renderer->matrix = &parent->matrix;

		return renderer;
	}

	PhysicsComponent* SceneManager::AddPhysicsBoxComponent(Vector3<PhysicsComponent::PhysicsComponent_t> pos, Quaternion<PhysicsComponent::PhysicsComponent_t> rot, PhysicMaterial mat, PhysicsComponent::BodyType type, Vector3<> size, SceneNode* parent)
	{
		PhysicsBoxComponent* component = new PhysicsBoxComponent(pos, rot, mat, type, size.x, size.y, size.z);
		
		parent->components.push_back(component);
		component->parent = parent;
		component->matrix = &parent->matrix;

		addedPhysicsComponents.push_back(component);
		return component;
	}

	Camera* SceneManager::AddCamera(SceneNode* parent)
	{
		Camera* cam = cameras.emplace_back(new Camera(XM_PI / 2.2, static_cast<float>(device->GetWidth()) / static_cast<float>(device->GetHeight()), 1.0f, 100000.0f));
		parent->components.push_back(cam);
		cam->parent = parent;
		cam->matrix = &parent->matrix;
		return cam;
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
	
	void SceneManager::SetMainCamera(Camera* camera)
	{
		assert(Alg::Exists(cameras, camera));
		
		mainCamera = camera;
	}

	void SceneManager::UpdateMatrices()
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

		// Update main camera
		if (mainCamera)
		{
			mainCamera->UpdateMatrices();
		}
	}

	void SceneManager::DrawAll(Engine const& engine)
	{
		Vector3<> camPos(5.0f, 5.0f, 5.0f);
		if (mainCamera)
		{
			// Todo: should have access to globalTransform here (there is only localTransform)
			camPos = Vector3<>(mainCamera->parent->matrix._14, mainCamera->parent->matrix._24, mainCamera->parent->matrix._34);
			
			for (auto& renderer : meshRenderers)
			{
				renderer->Draw(mainCamera->GetProjView(), camPos, shaders);
			}
		}
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
