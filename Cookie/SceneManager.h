#pragma once

#include "ExportMacro.h"
#include "Mesh.h"
#include "SceneNode.h"
#include "Device.h"
#include "MeshRenderer.h"
#include <stack>
#include "Util.h"
#include "Camera.h"

namespace Cookie
{
	class Engine;

	class COOKIE_API SceneManager
	{
	public:
		using SceneNodePtr = SceneNode::SceneNodePtr;
		using MeshPtr = Mesh*;
		SceneManager();

		// Todo: bof... comment découpler SceneManager et Device ?
		void SetDevice(Device* device);
		
		MeshPtr GetMesh(std::string const& filePath);

		// Components
		MeshRenderer* AddMeshRenderer(Mesh* mesh, Material* mat, SceneNode* parent);
		Camera* AddCamera(SceneNode* parent);
		
		SceneNodePtr GetRoot();
		SceneNodePtr AddSceneNode(SceneNodePtr parent);
		void RemoveSceneNode(SceneNodePtr node);
		template<class T>
		void AddComponent(T* component, SceneNodePtr parent);

		void SetMainCamera(Camera* camera);

		void UpdateMatrices();
		void DrawAll(Engine const& engine);
	private:
		static void UpdateNodeAndStackChildren(SceneNode* node, StackInsertIterator<std::stack<SceneNode*, std::vector<SceneNode*>>> insertIt);
		Device* device;
		SceneNode root;
		
		Camera* mainCamera;
		
		std::vector<MeshPtr> meshes;
		std::vector<SceneNode*> nodes;
		std::vector<MeshRenderer*> meshRenderers;
		std::vector<Camera*> cameras;
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
