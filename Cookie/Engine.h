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

	constexpr int TargetFps = 60;
	constexpr double frameTime = 1.0 / static_cast<double>(TargetFps);

	class COOKIE_API Engine
	{
	public:
		Engine(std::unique_ptr<Device>&& uninitializedDevice, std::unique_ptr<InputManager>&& uninitializedInputManager, std::unique_ptr<PhysicsEngine>&& uninitializedPhysicsEngine, std::unique_ptr<SceneManager>&& smgr);

		Device* GetDevice() const;
		SceneManager* GetSceneManager() const;
		TextureManager* GetTextureManager() const noexcept;

		/* Check that the device is still running and enqueue events from the system */
		bool Run();
		int Initialisations();

		DirectX::XMMATRIX const& GetMatView() const;
		DirectX::XMMATRIX const& GetMatProj() const;
		DirectX::XMMATRIX const& GetMatViewProj() const;

	private:
		bool Update();
		int InitScene();
		int InitAnimation();
		bool RenderScene();

	private:
		std::unique_ptr<Device> device;
		std::unique_ptr<InputManager> inputManager;
		std::unique_ptr<PhysicsEngine> physics;
		std::unique_ptr<SceneManager> smgr;
		std::unique_ptr<TextureManager> textureManager;

		int64_t TempsSuivant;
		int64_t TempsCompteurPrecedent;

		DirectX::XMMATRIX m_MatView;
		DirectX::XMMATRIX m_MatProj;
		DirectX::XMMATRIX m_MatViewProj;
	};
}
