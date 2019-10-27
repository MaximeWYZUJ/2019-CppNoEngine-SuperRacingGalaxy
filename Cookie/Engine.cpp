#include "pch.h"
#include "Engine.h"
#include <chrono>
#include <thread>

namespace Cookie
{
	using namespace std;
	using namespace DirectX;

	Engine::Engine(unique_ptr<Device>&& uninitializedDevice, unique_ptr<InputManager>&& uninitializedInputManager, unique_ptr<PhysicsEngine>&& uninitializedPhysicsEngine, unique_ptr<SceneManager>&& smgr)
		: device{ move(uninitializedDevice) }, inputManager{ move(uninitializedInputManager) }, physics{ move(uninitializedPhysicsEngine) }, smgr{ move(smgr) }
	{
	}

	Device* Engine::GetDevice() const
	{
		return device.get();
	}

	SceneManager* Engine::GetSceneManager() const
	{
		return smgr.get();
	}

	TextureManager* Engine::GetTextureManager() const noexcept
	{
		return textureManager.get();
	}

	bool Engine::Run()
	{
		bool isRunning = device->Run();
		inputManager->Update();

		physics->step();

		isRunning = Update();

		device->ClearEvents();
		
		return isRunning;
	}

	bool Engine::Update()
	{
		bool isCompleted = false;
		while (!isCompleted)
		{
			int64_t const currentTime = device->GetTimeSpecific();
			double const deltaTime = device->GetTimeIntervalsInSec(TempsCompteurPrecedent, currentTime);

			if (deltaTime > frameTime)
			{
				device->Present();
				RenderScene();
				TempsCompteurPrecedent = currentTime;
				isCompleted = true;
			}
			else if (deltaTime > 0.001) // > 1ms
			{
				this_thread::sleep_for(std::chrono::milliseconds(static_cast<int64_t>(deltaTime * 1000.0) - 1));
			}
		}

		return true;
	}

	int Engine::Initialisations()
	{
		device->Init(CdsMode::Windowed);
		inputManager->Init();
		physics->init();
		smgr->SetDevice(device.get());
		InitScene();
		InitAnimation();
		return 0;
	}

	const XMMATRIX& Engine::GetMatView() const
	{
		return m_MatView;
	}

	const XMMATRIX& Engine::GetMatProj() const
	{
		return m_MatProj;
	}

	const XMMATRIX& Engine::GetMatViewProj() const
	{
		return m_MatViewProj;
	}

	int Engine::InitScene()
	{
		m_MatView = XMMatrixLookAtLH(XMVectorSet(0.0f, 5.0f, -5.0f, 1.0f),
			XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f),
			XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
		float champDeVision = XM_PI / 2.2; // 45 degr�s
		const float ratioDAspect = static_cast<float>(device->GetWidth()) / static_cast<float>(device->GetHeight());
		float planRapproche = 2.0;
		float planEloigne = 1000.0;

		m_MatProj = XMMatrixPerspectiveFovLH(
			champDeVision,
			ratioDAspect,
			planRapproche,
			planEloigne);

		m_MatViewProj = m_MatView * m_MatProj;

		return 0;
	}

	int Engine::InitAnimation()
	{
		TempsSuivant = device->GetTimeSpecific();
		TempsCompteurPrecedent = TempsSuivant;

		RenderScene();

		return true;
	}

	bool Engine::RenderScene()
	{
		device->Clear(Color::Black);
		return smgr->DrawAll(*this);
	}
}
