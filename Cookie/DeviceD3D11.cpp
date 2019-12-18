#include "pch.h"
#include "resource.h"
#include "DeviceD3D11.h"
#include "Util.h"
#include "DeviceInfo.h"
#include "Color.h"
#include <string>

namespace Cookie
{
	using namespace std;
	
	DeviceD3D11::DeviceD3D11()
	{
		Init(CdsMode::Windowed);
	}

	DeviceD3D11::~DeviceD3D11()
	{
		// Todo: if init is not called destructor may fail
		pSwapChain->SetFullscreenState(FALSE, nullptr);

		if (pImmediateContext)
		{
			pImmediateContext->ClearState();
		}

		DXRelacher(mSolidCullBackRS);
		DXRelacher(pDepthStencilView);
		DXRelacher(pDepthTexture);
		DXRelacher(pRenderTargetView);
		DXRelacher(pImmediateContext);
		DXRelacher(pSwapChain);
		DXRelacher(device);
	}

	bool DeviceD3D11::Update()
	{
		MSG msg;
		bool shouldContinue = true;

		while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				shouldContinue = false;
			}

			if (!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}

		return shouldContinue;
	}

	int64_t DeviceD3D11::GetTimeSpecific() const
	{
		return horloge.GetTimeCount();
	}

	double DeviceD3D11::GetTimeIntervalsInSec(int64_t start, int64_t stop) const
	{
		return horloge.GetTimeBetweenCounts(start, stop);
	}

	int DeviceD3D11::Init(CdsMode cdsMode)
	{
		moduleHandle = GetCurrentModule();

		InitAppInstance();
		Show();

		UINT largeur;
		UINT hauteur;
		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL featureLevels[] =
		{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);
		pImmediateContext = NULL;
		pSwapChain = NULL;
		pRenderTargetView = NULL;
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		// Obtenir les informations de l’adaptateur de défaut
		DeviceInfo Dispo0(DeviceInfo::CURRENT_ADAPTER);
		largeur = 1024;
		hauteur = 768;
		switch (cdsMode)
		{
		case CdsMode::Windowed:
			sd.Windowed = TRUE;
			break;
		case CdsMode::FullScreen:
			sd.Windowed = FALSE;
			break;
		}
		DXGI_MODE_DESC desc;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.Height = hauteur;
		desc.Width = largeur;
		desc.RefreshRate.Numerator = 60;
		desc.RefreshRate.Denominator = 1;
		desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		DeviceInfo DispoVoulu(desc);
		DispoVoulu.GetDesc(desc);

		largeur = desc.Width;
		hauteur = desc.Height;

		screenWidth = largeur;
		screenHeight = hauteur;

		sd.BufferCount = 1;
		sd.BufferDesc.Width = desc.Width;
		sd.BufferDesc.Height = desc.Height;
		sd.BufferDesc.Format = desc.Format;
		sd.BufferDesc.RefreshRate.Numerator = desc.RefreshRate.Numerator;
		sd.BufferDesc.RefreshRate.Denominator = desc.RefreshRate.Denominator;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hMainWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		RECT rcClient, rcWindow;
		POINT ptDiff;
		GetClientRect(hMainWnd, &rcClient);
		GetWindowRect(hMainWnd, &rcWindow);
		ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
		ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
		MoveWindow(hMainWnd, rcWindow.left, rcWindow.top, largeur + ptDiff.x, hauteur + ptDiff.y, TRUE);

		DXEssayer(D3D11CreateDeviceAndSwapChain(0,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			createDeviceFlags,
			featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION,
			&sd,
			&pSwapChain,
			&device,
			NULL,
			&pImmediateContext),
			DXE_ERREURCREATIONDEVICE);

		ID3D11Texture2D* pBackBuffer;
		DXEssayer(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer), DXE_ERREUROBTENTIONBUFFER);
		DXEssayer(device->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView), DXE_ERREURCREATIONRENDERTARGET);
		pBackBuffer->Release();
		
		InitDepthBuffer();

		pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
		
		InitBlendStates();
		
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)largeur;
		vp.Height = (FLOAT)hauteur;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		pImmediateContext->RSSetViewports(1, &vp);

		D3D11_RASTERIZER_DESC rsDesc;
		ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_BACK;
		rsDesc.FrontCounterClockwise = false;
		device->CreateRasterizerState(&rsDesc, &mSolidCullBackRS);
		pImmediateContext->RSSetState(mSolidCullBackRS);

		return 0;
	}

	auto DeviceD3D11::CreateBuffer(BufferDescription const& bufferDescription, void const* data) -> BufferPointer
	{
		assert(bufferDescription.size > 0);
		
		D3D11_BUFFER_DESC desc;
		
		desc.ByteWidth = bufferDescription.size;
		desc.Usage = static_cast<D3D11_USAGE>(bufferDescription.GpuMemory);
		desc.BindFlags = static_cast<UINT>(bufferDescription.buffer);
		desc.CPUAccessFlags = static_cast<UINT>(bufferDescription.CpuAccess);
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subResData;
		subResData.pSysMem = data;
		subResData.SysMemPitch = 0;
		subResData.SysMemSlicePitch = 0;

		ID3D11Buffer* buffer;
		DXEssayer(device->CreateBuffer(&desc, &subResData, &buffer), DXE_CREATIONBUFFER);

		return BufferPointer{ reinterpret_cast<intptr_t>(buffer) };
	}

	auto DeviceD3D11::CreateTexture2D(Vector2<> size) -> Texture2D
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof desc);
		desc.Width = size.x;
		desc.Height = size.y;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		ID3D11Texture2D* result;
		ThrowOnError(device->CreateTexture2D(&desc, nullptr, &result));
		
		return Texture2D{ reinterpret_cast<intptr_t>(result) };
	}

	auto DeviceD3D11::CreateRenderTarget(Texture2D texture) -> RenderTarget
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc;
		
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		ID3D11RenderTargetView* result;
		ThrowOnError(device->CreateRenderTargetView(reinterpret_cast<ID3D11Texture2D*>(texture.p), &desc, &result));
		
		return RenderTarget{ reinterpret_cast<intptr_t>(result) };
	}

	auto DeviceD3D11::CreateShaderResource(Texture2D texture) -> ShaderResource
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;
		
		ID3D11ShaderResourceView* result;
		ThrowOnError(device->CreateShaderResourceView(reinterpret_cast<ID3D11Texture2D*>(texture.p), &desc, &result));
		
		return ShaderResource{ reinterpret_cast<intptr_t>(result) };
	}

	void DeviceD3D11::SetTopology()
	{
	}

	void DeviceD3D11::SetVertexBuffer(BufferPointer)
	{
	}

	void DeviceD3D11::SetIndexBuffer()
	{
	}

	void DeviceD3D11::SetRenderTargets(vector<RenderTarget> renderTargets)
	{
		ID3D11RenderTargetView* targets = reinterpret_cast<ID3D11RenderTargetView*>(renderTargets.data());
		pImmediateContext->OMSetRenderTargets(renderTargets.size(), &targets, pDepthStencilView);
	}

	void DeviceD3D11::ClearRenderTargets(vector<RenderTarget> renderTargets, Color clearColor)
	{
		for (auto& renderTarget : renderTargets)
		{
			pImmediateContext->ClearRenderTargetView(
				reinterpret_cast<ID3D11RenderTargetView*>(renderTarget.p),
				reinterpret_cast<FLOAT const*>(&clearColor));
		}
	}

	void DeviceD3D11::Draw(int32_t nbIndices)
	{
	}

	void DeviceD3D11::Clear(Color const& clearColor)
	{
		pImmediateContext->ClearRenderTargetView(pRenderTargetView, reinterpret_cast<FLOAT const*>(&clearColor));
		ResetDepthBuffer();
	}

	void DeviceD3D11::ResetDepthBuffer()
	{
		pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void DeviceD3D11::Present()
	{
		pSwapChain->Present(0, 0);
	}

	void DeviceD3D11::Release(Texture2D texture)
	{
		reinterpret_cast<ID3D11Texture2D*>(texture.p)->Release();
	}

	void DeviceD3D11::Release(RenderTarget renderTarget)
	{
		reinterpret_cast<ID3D11Texture2D*>(renderTarget.p)->Release();
	}

	void DeviceD3D11::Release(ShaderResource shaderResource)
	{
		reinterpret_cast<ID3D11Texture2D*>(shaderResource.p)->Release();
	}

	ID3D11Device* DeviceD3D11::GetD3DDevice() const
	{
		return device;
	}

	IDXGISwapChain* DeviceD3D11::GetSwapChain() const
	{
		return pSwapChain;
	}

	void DeviceD3D11::EnableAlphaBlend()
	{
		pImmediateContext->OMSetBlendState(alphaBlendEnable, nullptr, 0xffffffff);
	}

	void DeviceD3D11::DisableAlphaBlend()
	{
		pImmediateContext->OMSetBlendState(alphaBlendDisable, nullptr, 0xffffffff);
	}
	
	HMODULE DeviceD3D11::GetModule() const
	{
		return moduleHandle;
	}

	HWND DeviceD3D11::GetWindow() const
	{
		return hMainWnd;
	}

	ATOM DeviceD3D11::MyRegisterClass(HINSTANCE hInstance)
	{
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = &WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_Cookie));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = MAKEINTRESOURCE(IDC_Cookie);
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		return RegisterClassEx(&wcex);
	}

	bool DeviceD3D11::InitAppInstance()
	{
		TCHAR szTitle[MAX_LOADSTRING];

		LoadString(moduleHandle, IDS_APP_TITLE, static_cast<TCHAR*>(szTitle), MAX_LOADSTRING);
		LoadString(moduleHandle, IDC_Cookie, static_cast<TCHAR*>(szWindowClass), MAX_LOADSTRING);

		if (!MyRegisterClass(moduleHandle))
		{
			MessageBox(NULL, L"Class registration has failed!", L"Error!", MB_OK | MB_ICONINFORMATION);
			return false;
		}

		hMainWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, moduleHandle, nullptr);

		if (!hMainWnd)
		{
			MessageBox(NULL, L"Window creation failed!", L"Error!", MB_OK | MB_ICONINFORMATION);
			return false;
		}

		SetWindowLongPtr(hMainWnd, GWLP_USERDATA, (LONG_PTR)this);

		hAccelTable = LoadAccelerators(moduleHandle, MAKEINTRESOURCE(IDC_Cookie));

		return true;
	}

	int DeviceD3D11::Show()
	{
		ShowWindow(hMainWnd, SW_SHOWNORMAL);
		UpdateWindow(hMainWnd);

		return 0;
	}

	HMODULE DeviceD3D11::GetCurrentModule()
	{
		HMODULE hModule = nullptr;
		GetModuleHandleEx(
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
			reinterpret_cast<LPCTSTR>(GetCurrentModule),
			&hModule);

		return hModule;
	}

	void DeviceD3D11::ThrowOnError(HRESULT result)
	{
		if (FAILED(result))
		{
			throw std::exception("Direct3D ThrowOnError");
		}
	}

	void DeviceD3D11::InitDepthBuffer()
	{
		D3D11_TEXTURE2D_DESC depthTextureDesc;
		ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
		depthTextureDesc.Width = screenWidth;
		depthTextureDesc.Height = screenHeight;
		depthTextureDesc.MipLevels = 1;
		depthTextureDesc.ArraySize = 1;
		depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTextureDesc.SampleDesc.Count = 1;
		depthTextureDesc.SampleDesc.Quality = 0;
		depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTextureDesc.CPUAccessFlags = 0;
		depthTextureDesc.MiscFlags = 0;

		DXEssayer(device->CreateTexture2D(&depthTextureDesc,
			NULL, &pDepthTexture),
			DXE_ERREURCREATIONTEXTURE);

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSView;
		ZeroMemory(&descDSView, sizeof(descDSView));

		descDSView.Format = depthTextureDesc.Format;
		descDSView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSView.Texture2D.MipSlice = 0;

		DXEssayer(device->CreateDepthStencilView(pDepthTexture,
			&descDSView,
			&pDepthStencilView),
			DXE_ERREURCREATIONDEPTHSTENCILTARGET);
	}

	void DeviceD3D11::disableZBuffer()
	{
		D3D11_DEPTH_STENCIL_DESC dsDesc;

		// Depth test parameters
		dsDesc.DepthEnable = false;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		// Stencil test parameters
		dsDesc.StencilEnable = false;
		dsDesc.StencilReadMask = 0xFF;
		dsDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create depth stencil state
		ID3D11DepthStencilState* pDSState;
		device->CreateDepthStencilState(&dsDesc, &pDSState);

		// Disable Z-Buffer
		pImmediateContext->OMSetDepthStencilState(pDSState, 1);
	}

	void DeviceD3D11::enableZBuffer()
	{
		D3D11_DEPTH_STENCIL_DESC dsDesc;

		// Depth test parameters
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		// Stencil test parameters
		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = 0xFF;
		dsDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create depth stencil state
		ID3D11DepthStencilState* pDSState;
		device->CreateDepthStencilState(&dsDesc, &pDSState);

		// Disable Z-Buffer
		pImmediateContext->OMSetDepthStencilState(pDSState, 1);
	}

	ID3D11DepthStencilView* DeviceD3D11::GetDepthStencilView()
	{
		return pDepthStencilView;
	}

	ID3D11RenderTargetView* DeviceD3D11::GetRenderTargetView()
	{
		return pRenderTargetView;
	}

	void DeviceD3D11::SetRenderTargetView(ID3D11RenderTargetView* targetView, ID3D11DepthStencilView* depthStencilView)
	{
		pRenderTargetView = targetView;
		pDepthStencilView = depthStencilView;

		ID3D11RenderTargetView* tabRTV[1];
		tabRTV[0] = pRenderTargetView;
		pImmediateContext->OMSetRenderTargets(1, tabRTV, pDepthStencilView);
	}
	
	void DeviceD3D11::InitBlendStates()
	{
		D3D11_BLEND_DESC blendDesc;

		// Effacer la description
		ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

		// On initialise la description pour un mélange alpha classique
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		
		// On créé l'état alphaBlendEnable
		DXEssayer(device->CreateBlendState(&blendDesc, &alphaBlendEnable), DXE_ERREURCREATION_BLENDSTATE);
		
		// Seul le booleen BlendEnable nécéssite d'être modifié
		blendDesc.RenderTarget[0].BlendEnable = FALSE;

		// On créé l'état alphaBlendDisable
		DXEssayer(device->CreateBlendState(&blendDesc, &alphaBlendDisable), DXE_ERREURCREATION_BLENDSTATE);
	}


	LRESULT CALLBACK DeviceD3D11::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		auto* c = reinterpret_cast<DeviceD3D11*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if (c == nullptr)
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		int wmId, wmEvent;
		PAINTSTRUCT ps;
		HDC hdc;

		switch (message)
		{
		case WM_SETFOCUS:
			c->events.push_back(DeviceEvent(DeviceEventType::Focus));
			c->hasFocus = true;
			break;
		case WM_KILLFOCUS:
			c->events.push_back(DeviceEvent(DeviceEventType::FocusLost));
			c->hasFocus = false;
			break;
		case WM_MOUSEMOVE:
			c->events.push_back(DeviceEvent(DeviceEventType::MouseMove, MouseMoveData{ .pos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) } }));
			break;
		case WM_LBUTTONDOWN:
			c->events.push_back(DeviceEvent(DeviceEventType::MouseButton, MouseButtonData(MouseButtonEventType::LeftButtonDown, { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) })));
			break;
		case WM_LBUTTONUP:
			c->events.push_back(DeviceEvent(DeviceEventType::MouseButton, MouseButtonData(MouseButtonEventType::LeftButtonUp, { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) })));
			break;
		case WM_RBUTTONDOWN:
			c->events.push_back(DeviceEvent(DeviceEventType::MouseButton, MouseButtonData(MouseButtonEventType::RightButtonDown, { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) })));
			break;
		case WM_RBUTTONUP:
			c->events.push_back(DeviceEvent(DeviceEventType::MouseButton, MouseButtonData(MouseButtonEventType::RightButtonUp, { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) })));
			break;
		case WM_MOUSEWHEEL:
			c->events.push_back(DeviceEvent(DeviceEventType::MouseWheel, MouseWheelData(GET_WHEEL_DELTA_WPARAM(wParam) / static_cast<float>(WHEEL_DELTA))));
			break;
		case WM_COMMAND:
			wmId = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(c->GetModule(), (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

	INT_PTR CALLBACK DeviceD3D11::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);
		switch (message)
		{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
		}
		return (INT_PTR)FALSE;
	}
}
