#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Shaders.h"

namespace Cookie
{
	class DeviceD3D11;

	class CSommetPanneauPE
	{
	public:
		CSommetPanneauPE() = default;
		CSommetPanneauPE(const Vector3<>& position, const Vector2<>& coordTex) :
			m_position(position),
			m_coordTex(coordTex)
		{
			
		}
		
		static UINT numElements;
		static D3D11_INPUT_ELEMENT_DESC layout[];
		Vector3<> m_position;
		Vector2<> m_coordTex;
	};

	class PostEffectPanel
	{
	public:
		explicit PostEffectPanel(DeviceD3D11* device);
		virtual ~PostEffectPanel();
		virtual void Draw();
		void BeginPostEffect();
		void EndPostEffect();
		
	private:
		void InitPostEffect();
		static CSommetPanneauPE sommets[6];
		ID3D11Buffer* pVertexBuffer;
		DeviceD3D11* device;

		Shaders shader;

		// Texture de rendu pour effets
		ID3D11Texture2D* pTextureScene;
		ID3D11ShaderResourceView* pResourceView;
		ID3D11Texture2D* pDepthTexture;
		ID3D11RenderTargetView* pRenderTargetView;
		ID3D11DepthStencilView* pDepthStencilView ;

	protected:
		ID3D11RenderTargetView* pOldRenderTargetView;
		ID3D11DepthStencilView* pOldDepthStencilView;

	};
}
