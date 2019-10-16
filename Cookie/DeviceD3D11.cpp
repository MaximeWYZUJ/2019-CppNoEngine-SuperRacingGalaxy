#include "pch.h"
#include "resource.h"
#include "DeviceD3D11.h"
#include "Util.h"
#include "DeviceInfo.h"
#include "Color.h"

namespace Cookie
{
	HINSTANCE DeviceD3D11::hAppInstance;

	DeviceD3D11::DeviceD3D11()
	{
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

	bool DeviceD3D11::Run()
	{
		MSG msg;
		bool bBoucle = true;

		if (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) bBoucle = false;

			if (!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}

		return bBoucle;
	}

	int64_t DeviceD3D11::GetTimeSpecific() const
	{
		return m_Horloge.GetTimeCount();
	}

	double DeviceD3D11::GetTimeIntervalsInSec(int64_t start, int64_t stop) const
	{
		return m_Horloge.GetTimeBetweenCounts(start, stop);
	}

	int DeviceD3D11::Init(CdsMode cdsMode, HMODULE hModule)
	{
		hAppInstance = hModule;

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
		// Obtenir les informations de l�adaptateur de d�faut
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

		return reinterpret_cast<BufferPointer>(buffer);
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

	void DeviceD3D11::Draw(int32_t nbIndices)
	{
	}

	void DeviceD3D11::Clear(Color const& clearColor)
	{
		pImmediateContext->ClearRenderTargetView(pRenderTargetView, reinterpret_cast<FLOAT const*>(&clearColor));
		pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void DeviceD3D11::Present()
	{
		pSwapChain->Present(0, 0);
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
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALLENGINE));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = MAKEINTRESOURCE(IDC_SMALLENGINE);
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		return RegisterClassEx(&wcex);
	}

	bool DeviceD3D11::InitAppInstance()
	{
		TCHAR szTitle[MAX_LOADSTRING];

		LoadString(hAppInstance, IDS_APP_TITLE, static_cast<TCHAR*>(szTitle), MAX_LOADSTRING);
		LoadString(hAppInstance, IDC_SMALLENGINE, static_cast<TCHAR*>(szWindowClass), MAX_LOADSTRING);

		if (!MyRegisterClass(hAppInstance))
		{
			MessageBox(NULL, L"Class registration has failed!", L"Error!", MB_OK | MB_ICONINFORMATION);
			return false;
		}

		hMainWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hAppInstance, nullptr);

		if (!hMainWnd)
		{
			MessageBox(NULL, L"Window creation failed!", L"Error!", MB_OK | MB_ICONINFORMATION);
			return false;
		}

		hAccelTable = LoadAccelerators(hAppInstance, MAKEINTRESOURCE(IDC_SMALLENGINE));

		return true;
	}

	int DeviceD3D11::Show()
	{
		ShowWindow(hMainWnd, SW_SHOWNORMAL);
		UpdateWindow(hMainWnd);

		return 0;
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

	LRESULT CALLBACK DeviceD3D11::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		int wmId, wmEvent;
		PAINTSTRUCT ps;
		HDC hdc;

		switch (message)
		{
		case WM_COMMAND:
			wmId = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hAppInstance, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
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
			// �vitez d'ajouter du code ici...
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
