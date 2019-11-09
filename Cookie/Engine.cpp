#include "pch.h"
#include "Engine.h"
#include <chrono>
#include <thread>

namespace Cookie
{
	using namespace std;
	using namespace DirectX;

	Engine::Engine(
		unique_ptr<Device>&& uninitializedDevice,
		unique_ptr<InputManager>&& uninitializedInputManager,
		unique_ptr<ActionManager>&& actionManager,
		unique_ptr<PhysicsEngine>&& uninitializedPhysicsEngine,
		unique_ptr<SceneManager>&& smgr,
		unique_ptr<TextureManager>&& tm,
		unique_ptr<MaterialManager>&& mm)
		:
		device{ move(uninitializedDevice) },
		inputManager{ move(uninitializedInputManager) },
		actionManager{ move(actionManager) },
		physics{ move(uninitializedPhysicsEngine) },
		sceneManager{ move(smgr) },
		textureManager{ move(tm) },
		materialManager { move(mm) }
	{
		device->Init(CdsMode::Windowed);
		inputManager->Init();
		physics->init();
		sceneManager->SetDevice(device.get());
		InitScene();
		InitAnimation();
	}

	Device* Engine::GetDevice() const
	{
		return device.get();
	}

	InputManager* Engine::GetInputManager() const
	{
		return inputManager.get();
	}

	ActionManager* Engine::GetActionManager() const
	{
		return actionManager.get();
	}

	SceneManager* Engine::GetSceneManager() const
	{
		return sceneManager.get();
	}

	TextureManager* Engine::GetTextureManager() const noexcept
	{
		return textureManager.get();
	}

	MaterialManager* Engine::GetMaterialManager() const noexcept
	{
		return materialManager.get();
	}
	
	Matrix4x4<> const& Engine::GetMatView() const
	{
		return m_MatView;
	}

	Matrix4x4<> const& Engine::GetMatProj() const
	{
		return m_MatProj;
	}

	Matrix4x4<> const& Engine::GetMatViewProj() const
	{
		return m_MatViewProj;
	}

	int Engine::InitScene()
	{
		float champDeVision = XM_PI / 2.2; // 45 degrés
		const float ratioDAspect = static_cast<float>(device->GetWidth()) / static_cast<float>(device->GetHeight());
		float planRapproche = 2.0;
		float planEloigne = 1000.0;

		m_MatView = Matrix4x4<>::FromLookAt({ 0.0f, 5.0f, -5.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
		m_MatProj = Matrix4x4<>::FromPerspective(champDeVision, ratioDAspect, planRapproche, planEloigne);

		m_MatViewProj = m_MatProj * m_MatView;

		return 0;
	}

	int Engine::InitAnimation()
	{
		previousTime = device->GetTimeSpecific();

		RenderScene();

		return true;
	}

	void Engine::UpdateScene()
	{
		bool isCompleted = false;
		while (!isCompleted)
		{
			int64_t const currentTime = device->GetTimeSpecific();
			double const deltaTime = device->GetTimeIntervalsInSec(previousTime, currentTime);

			if (deltaTime > frameTime)
			{
				device->Present();
				RenderScene();
				previousTime = currentTime;
				isCompleted = true;
			}
			else if (deltaTime > 0.001) // > 1ms
			{
				this_thread::sleep_for(std::chrono::milliseconds(static_cast<int64_t>(deltaTime * 1000.0) - 1));
			}
		}
	}

	void Engine::RenderScene() const
	{
		device->Clear(Color::Black);
		sceneManager->DrawAll(*this);
	}
}
