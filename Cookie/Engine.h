#pragma once

#include <memory>

#include "ExportMacro.h"
#include "Color.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "PhysicsEngine.h"

namespace Cookie
{
	class Device;

	constexpr int targetFps = 60;
	constexpr double frameTime = 1.0 / static_cast<double>(targetFps);

	class COOKIE_API Engine
	{
	public:
		Engine(std::unique_ptr<Device>&& uninitializedDevice, std::unique_ptr<InputManager>&& uninitializedInputManager, std::unique_ptr<PhysicsEngine>&& uninitializedPhysicsEngine, std::unique_ptr<SceneManager>&& smgr);

		template<class TUpdateFunc>
		bool Run(TUpdateFunc update);

		Device* GetDevice() const;
		SceneManager* GetSceneManager() const;
		TextureManager* GetTextureManager() const noexcept;

		DirectX::XMMATRIX const& GetMatView() const;
		DirectX::XMMATRIX const& GetMatProj() const;
		DirectX::XMMATRIX const& GetMatViewProj() const;

	private:
		int InitScene();
		int InitAnimation();
		void UpdateScene();
		void RenderScene() const;

		std::unique_ptr<Device> device;
		std::unique_ptr<InputManager> inputManager;
		std::unique_ptr<PhysicsEngine> physics;
		std::unique_ptr<SceneManager> sceneManager;
		std::unique_ptr<TextureManager> textureManager;

		int64_t previousTime;

		DirectX::XMMATRIX m_MatView;
		DirectX::XMMATRIX m_MatProj;
		DirectX::XMMATRIX m_MatViewProj;
	};

	template<class TUpdateFunc>
	bool Engine::Run(TUpdateFunc update)
	{
		if (!device->Update())
		{
			return false;
		}

		inputManager->Update();
		physics->step();
		// synchronizer->Update() should be here

		update();

		UpdateScene();

		inputManager->PostUpdate();
		device->PostUpdate();

		return true;
	}
}
