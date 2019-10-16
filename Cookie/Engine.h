#pragma once

#include <memory>

#include "ExportMacro.h"
#include "Color.h"
#include "SceneManager.h"
#include "TextureManager.h"

namespace SmallEngine
{
	class Device;

	constexpr int TargetFps = 60;
	constexpr double EcartTemps = 1.0 / static_cast<double>(TargetFps);

	class COOKIE_API Engine
	{
	public:
		Engine(std::unique_ptr<Device>&& uninitializedDevice, std::unique_ptr<SceneManager>&& smgr);

		Device* GetDevice() const;
		SceneManager* GetSceneManager() const;
		TextureManager* GetTextureManager() const noexcept;

		/* Check that the device is still running and enqueue events from the system */
		bool Run();
		bool Update();
		int Initialisations();
		bool Animation();

		DirectX::XMMATRIX const& GetMatView() const;
		DirectX::XMMATRIX const& GetMatProj() const;
		DirectX::XMMATRIX const& GetMatViewProj() const;

	private:
		int InitScene();
		int InitAnimation();
		bool AnimeScene(float tempsEcoule);
		bool RenderScene();

	private:
		std::unique_ptr<Device> device;
		std::unique_ptr<SceneManager> smgr;
		std::unique_ptr<TextureManager> textureManager;

		int64_t TempsSuivant;
		int64_t TempsCompteurPrecedent;

		DirectX::XMMATRIX m_MatView;
		DirectX::XMMATRIX m_MatProj;
		DirectX::XMMATRIX m_MatViewProj;

		static HMODULE GetCurrentModule();
	};
}