#pragma once

#include "ExportMacro.h"
#include "Mesh.h"
#include "SceneNode.h"
#include "Device.h"
#include "MeshRenderer.h"
#include <stack>
#include "Util.h"
#include "Camera.h"
#include "Shaders.h"
#include "PhysicsComponent.h"
#include "DeferredShadingPipeline.h"
#include "BillBoard.h"

namespace Cookie
{
	class Engine;

	class COOKIE_API SceneManager
	{
	public:
		using SceneNodePtr = SceneNode::SceneNodePtr;
		using MeshPtr = Mesh*;
		SceneManager(Device* device);
		
		MeshPtr GetMesh(std::string const& filePath);

		// Components
		MeshRenderer* AddMeshRenderer(Mesh* mesh, Material* mat, SceneNode* parent);
		MeshRenderer* AddMeshRenderer(Mesh* mesh, Material* mat, SceneNode* parent, int priority);
		PhysicsComponent* AddPhysicsBoxComponent(
			PhysicMaterial mat,
			PhysicsComponent::BodyType type,
			SceneNode* parent,
			bool trigger = false);
		PhysicsComponent* AddPhysicsSphereComponent(
			PhysicMaterial mat,
			PhysicsComponent::BodyType type,
			SceneNode* parent,
			bool trigger = false);
		PhysicsComponent* AddPhysicsMeshComponent(
			PhysicMaterial mat,
			PhysicsComponent::BodyType type,
			Mesh& mesh,
			SceneNode* parent,
			bool trigger = false);
		Camera* AddCamera(SceneNode* parent);
		Billboard* AddBillboard(SceneNode* parent);
		
		SceneNodePtr GetRoot();
		SceneNodePtr AddSceneNode(SceneNodePtr parent);
		void RemoveSceneNode(SceneNodePtr node);
		template<class T>
		void AddComponent(T* component, SceneNodePtr parent);

		void SetMainCamera(Camera* camera);
		Camera* GetMainCamera() const;

		void UpdateMatrices();
		void DrawAll(Engine const& engine);


		std::vector<PhysicsComponent*> addedPhysicsComponents;

	private:
		static void UpdateNodeAndStackChildren(SceneNode* node, StackInsertIterator<std::stack<SceneNode*, std::vector<SceneNode*>>> insertIt);
		void RenderPass(std::vector<MeshRenderer*> const& renderers, Vector3<> const& camPos) const;
		Device* device;
		SceneNode root;
		
		Camera* mainCamera;

		Shaders shaders; // standard shader
		Shaders planetShader; // blending 2 textures shader based on alpha (3rd texture)
		DeferredShadingPipeline shading;
		
		std::vector<MeshPtr> meshes;
		std::vector<SceneNode*> nodes;
		std::vector<MeshRenderer*> meshRenderers;
		std::vector<Camera*> cameras;
		std::vector<Billboard*> billboards;

		std::vector<MeshRenderer*> firstPassMeshRenderers;
		std::vector<MeshRenderer*> billboardPassMeshRenderers;
		std::vector<MeshRenderer*> finalPassMeshRenderers;
	};
	
	template<class T>
	void SceneManager::AddComponent(T* component, SceneNodePtr parent)
	{
		throw std::exception("Component type is not supported!");
	}

	template<>
	inline void SceneManager::AddComponent(MeshRenderer* component, SceneNodePtr parent)
	{
		meshRenderers.push_back(component);
		parent->components.push_back(component);
	}
}
