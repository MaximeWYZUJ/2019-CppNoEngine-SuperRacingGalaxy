#pragma once

#include <memory>

#include "ExportMacro.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "PhysicsEngine.h"
#include "MaterialManager.h"
#include "ActionManager.h"
#include "Synchronizer.h"

namespace Cookie
{
	class Device;

	constexpr int targetFps = 60;
	constexpr double frameTime = 1.0 / static_cast<double>(targetFps);

	class COOKIE_API Engine
	{
	public:
		Engine(
			std::unique_ptr<Device>&& device,
			std::unique_ptr<InputManager>&& inputManager,
			std::unique_ptr<ActionManager>&& actionManager,
			std::unique_ptr<PhysicsEngine>&& physicsEngine,
			std::unique_ptr<SceneManager>&& smgr,
			std::unique_ptr<TextureManager>&& textureManager,
			std::unique_ptr<MaterialManager>&& materialManager,
			std::unique_ptr<Synchronizer>&& synchronizer);

		template<class TUpdateFunc>
		bool Run(TUpdateFunc update);

		Device* GetDevice() const;
		InputManager* GetInputManager() const;
		ActionManager* GetActionManager() const;
		PhysicsEngine* GetPhysicsEngine() const;
		SceneManager* GetSceneManager() const;
		TextureManager* GetTextureManager() const noexcept;
		MaterialManager* GetMaterialManager() const noexcept;

		Matrix4x4<> const& GetMatView() const;
		Matrix4x4<> const& GetMatProj() const;
		Matrix4x4<> const& GetMatViewProj() const;

	private:
		int InitScene();
		int InitAnimation();
		void UpdateScene();
		void RenderScene() const;

		std::unique_ptr<Device> device;
		std::unique_ptr<InputManager> inputManager;
		std::unique_ptr<ActionManager> actionManager;
		std::unique_ptr<PhysicsEngine> physics;
		std::unique_ptr<SceneManager> sceneManager;
		std::unique_ptr<TextureManager> textureManager;
		std::unique_ptr<MaterialManager> materialManager;
		std::unique_ptr<Synchronizer> synchronizer;

		int64_t previousTime;

		Matrix4x4<> m_MatView;
		Matrix4x4<> m_MatProj;
		Matrix4x4<> m_MatViewProj;
	};

	template<class TUpdateFunc>
	bool Engine::Run(TUpdateFunc update)
	{
		if (!device->Update())
		{
			return false;
		}

		// Physics
		physics->step();
		synchronizer->SyncUp();

		// Input Events
		inputManager->Update();
		actionManager->Update();

		// Game Logic
		update();
		sceneManager->UpdateMatrices();
		synchronizer->SyncDown(sceneManager);

		// Scene Rendering
		UpdateScene();

		// GUI Rendering
		// Todo: 

		// Post Updates
		inputManager->PostUpdate();
		device->PostUpdate();

		return true;
	}
}
