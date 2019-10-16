#pragma once

#include "ExportMacro.h"
#include "Mesh.h"
#include "SceneNode.h"
#include "Device.h"
#include "MeshRenderer.h"

namespace Cookie
{
	class Engine;

	class COOKIE_API SceneManager
	{
	public:
		using SceneNodePtr = SceneNode::SceneNodePtr;
		using MeshPtr = Mesh*;
		SceneManager();

		// Todo: bof... comment d�coupler SceneManager et Device ?
		void SetDevice(Device* device);
		
		MeshPtr GetMesh(std::string const& filePath);

		// Components
		MeshRenderer* AddMeshRenderer(Mesh* mesh, Material* mat, SceneNode* parent);

		SceneNodePtr GetRoot();
		SceneNodePtr AddSceneNode(SceneNodePtr parent);
		void RemoveSceneNode(SceneNodePtr node);
		template<class T>
		void AddComponent(T* component, SceneNodePtr parent);

		bool DrawAll(Engine const& engine);
	private:
		Device* device;
		SceneNode root;
		std::vector<MeshPtr> meshes;
		std::vector<SceneNode*> nodes;
		std::vector<MeshRenderer*> meshRenderers;
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
		parent->Components.push_back(component);
	}
}
