#pragma once

#include <string_view>
#include "ExportMacro.h"
#include "Mesh.h"
#include "SceneNode.h"
#include "Device.h"

namespace Cookie
{
	class Engine;

	class COOKIE_API SceneManager
	{
	public:
		SceneManager();

		// Todo: bof... comment découpler SceneManager et Device ?
		void SetDevice(Device* device);

		Mesh* GetMesh(std::string const& filePath);
		SceneNode* GetRoot();
		SceneNode* CreateSceneNode();

		bool AnimateAll();
		bool DrawAll(Engine const& engine);
	private:
		Device* device;
		SceneNode root;
		std::vector<SceneNode> sceneNodes;
		std::vector<Mesh> meshes;
	};
}