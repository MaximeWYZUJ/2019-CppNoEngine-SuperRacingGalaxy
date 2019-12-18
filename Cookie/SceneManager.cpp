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
#include "MiniPhongParams.h"
#include "Layout.h"

#undef max

using namespace DirectX;

namespace Cookie
{
	using namespace std;

	SceneManager::SceneManager(Device* device) : device{ device }, shaders{ device, L"MiniPhong", sizeof MiniPhongParams, VertexData::layout, VertexData::nbElements }
	{
		meshes.reserve(1024);
		root.localMatrix = Matrix4x4<>::FromTransform(root.localTransform);
		root.matrix = root.localMatrix;
	}

	Mesh* SceneManager::GetMesh(string const& filePath)
	{
		auto it = find_if(meshes.begin(), meshes.end(), [&filePath](auto& mesh) { return mesh->GetFilePath() == filePath; });
		if (it != meshes.end())
		{
			return *it;
		}

		ObjData res = ObjReader::Read(filePath);
		MeshPtr meshRes = ObjDataToMeshConverter::Convert(res, filePath);
		Mesh* mesh = meshes.emplace_back(meshRes);

		return mesh;
	}

	MeshRenderer* SceneManager::AddMeshRenderer(Mesh* mesh, Material* mat, SceneNode* parent)
	{
		MeshRenderer* renderer = meshRenderers.emplace_back(new MeshRenderer(mesh, mat, device));

		parent->components.push_back(renderer);
		renderer->sceneNode = parent;
		renderer->matrix = &parent->matrix;

		return renderer;
	}

	PhysicsComponent* SceneManager::AddPhysicsBoxComponent(PhysicMaterial mat, PhysicsComponent::BodyType type, SceneNode* parent, bool trigger)
	{
		PhysicsBoxComponent* component = new PhysicsBoxComponent(
			parent->localTransform.GetPosition(),
			parent->localTransform.GetRotation(),
			mat,
			type,
			parent->localTransform.GetScale().x,
			parent->localTransform.GetScale().y,
			parent->localTransform.GetScale().z,
			trigger);

		parent->components.push_back(component);
		component->sceneNode = parent;
		component->matrix = &parent->matrix;

		addedPhysicsComponents.push_back(component);
		return component;
	}

	PhysicsComponent* SceneManager::AddPhysicsSphereComponent(PhysicMaterial mat, PhysicsComponent::BodyType type, SceneNode* parent, bool trigger)
	{
		PhysicsSphereComponent* component = new PhysicsSphereComponent(
			parent->localTransform.GetPosition(),
			parent->localTransform.GetRotation(),
			mat,
			type,
			parent->localTransform.GetScale().x,
			trigger);

		parent->components.push_back(component);
		component->sceneNode = parent;
		component->matrix = &parent->matrix;

		addedPhysicsComponents.push_back(component);
		return component;
	}

	PhysicsComponent* SceneManager::AddPhysicsMeshComponent(PhysicMaterial mat, PhysicsComponent::BodyType type, Mesh& mesh, SceneNode* parent, bool trigger)
	{
		PhysicsMeshComponent* component = new PhysicsMeshComponent(
			parent->localTransform.GetPosition(),
			parent->localTransform.GetRotation(),
			mat,
			type,
			mesh,
			parent->localTransform.GetScale(),
			trigger);

		parent->components.push_back(component);
		component->sceneNode = parent;
		component->matrix = &parent->matrix;

		addedPhysicsComponents.push_back(component);
		return component;
	}

	Camera* SceneManager::AddCamera(SceneNode* parent)
	{
		Camera* cam = cameras.emplace_back(new Camera(XM_PI / 2.2, static_cast<float>(device->GetWidth()) / static_cast<float>(device->GetHeight()), 1.0f, 100'000.0f));
		parent->components.push_back(cam);
		cam->sceneNode = parent;
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
		assert(Algo::Exists(cameras, camera));

		mainCamera = camera;
	}

	Camera* SceneManager::GetMainCamera() const
	{
		return mainCamera;
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
		if (mainCamera)
		{
			// Todo: should have access to globalTransform here (there is only localTransform)
			Vector3<> camPos = Vector3<>(mainCamera->sceneNode->matrix._14, mainCamera->sceneNode->matrix._24, mainCamera->sceneNode->matrix._34);

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