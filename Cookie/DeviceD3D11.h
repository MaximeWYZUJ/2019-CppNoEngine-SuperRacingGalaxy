#pragma once

#include "Device.h"
#include "Horloge.h"

#define MAX_LOADSTRING 100

namespace Cookie
{
	class DeviceD3D11 final : public Device
	{
	public:
		DeviceD3D11();
		virtual ~DeviceD3D11();

		bool Update() override;
		int64_t GetTimeSpecific() const override;
		double GetTimeIntervalsInSec(int64_t start, int64_t stop) const override;
		int Init(CdsMode cdsMode) override;

		// Direct3D function wrappers
		BufferPointer CreateBuffer(BufferDescription const& bufferDescription, void const* data) override;
		void SetTopology() override;
		void SetVertexBuffer(BufferPointer) override;
		void SetIndexBuffer() override;
		void Draw(int32_t nbIndices) override;
		
		void Clear(Color const& clearColor) override;
		void ResetDepthBuffer();
		void Present() override;

		ID3D11Device* GetD3DDevice() const;
		IDXGISwapChain* GetSwapChain() const;

		void EnableAlphaBlend();
		void DisableAlphaBlend();
		
		HMODULE GetModule() const;
		HWND GetWindow() const;

		void enableZBuffer();
		void disableZBuffer();

		ID3D11RenderTargetView* GetRenderTargetView();
		ID3D11DepthStencilView* GetDepthStencilView();
		void SetRenderTargetView(ID3D11RenderTargetView* targetView, ID3D11DepthStencilView* depthStencilView);
			
	protected:
		ID3D11RasterizerState* mSolidCullBackRS;

	private:
		// Window section
		ATOM MyRegisterClass(HINSTANCE hInstance);
		bool InitAppInstance();
		int Show();
		static HMODULE GetCurrentModule();

	private:
		void InitDepthBuffer();
		void InitBlendStates();

	private:
		ID3D11Device * device;
		ID3D11DeviceContext*    pImmediateContext;
		IDXGISwapChain*         pSwapChain;
		ID3D11RenderTargetView* pRenderTargetView;
		ID3D11Texture2D* pDepthTexture;
		ID3D11DepthStencilView* pDepthStencilView;

		//Alpha blend
		ID3D11BlendState* alphaBlendEnable;
		ID3D11BlendState* alphaBlendDisable;

	private:
		// Window section
		static LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
		static INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

		HACCEL hAccelTable;						// handle Windows de la table des accélérateurs
		HMODULE moduleHandle;			// handle Windows de l'instance actuelle de l'application
		HWND hMainWnd;							// handle Windows de la fenêtre principale
		TCHAR szWindowClass[MAX_LOADSTRING];	// le nom de la classe de fenêtre principale

		Horloge horloge;
	};

}