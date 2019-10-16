#include "pch.h"

#include <vector>

#include "DeviceInfo.h"
#include "util.h"

namespace Cookie
{
	using namespace std;

	DeviceInfo::DeviceInfo(DXGI_MODE_DESC modeDesc)
	{
		IDXGIFactory* pFactory = nullptr;
		CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
		IDXGIAdapter* pAdapter;
		vector<IDXGIAdapter*> vAdapters;
		for (UINT i = 0;
			pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			vAdapters.push_back(pAdapter);
		}
		*this = DeviceInfo(0);
		IDXGIOutput* pOutput = nullptr;
		vAdapters[0]->EnumOutputs(0, &pOutput);
		pOutput->FindClosestMatchingMode(&modeDesc, &mode, nullptr);
		DXRelacher(pOutput);
		for (int i = 0; i < vAdapters.size(); ++i)
		{
			DXRelacher(vAdapters[i]);
		}
		DXRelacher(pFactory);
	}

	DeviceInfo::DeviceInfo(int NoAdaptateur)
	{
		IDXGIFactory* pFactory = nullptr;
		IDXGIAdapter* pAdapter = nullptr;
		IDXGIOutput* pOutput = nullptr;
		isValid = false;
		CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pFactory));
		if (FAILED(pFactory->EnumAdapters(NoAdaptateur, &pAdapter))) return;
		if (FAILED(pAdapter->EnumOutputs(0, &pOutput))) return;
		DXGI_OUTPUT_DESC outDesc;
		pOutput->GetDesc(&outDesc);
		width = outDesc.DesktopCoordinates.right - outDesc.DesktopCoordinates.left;
		height = outDesc.DesktopCoordinates.bottom - outDesc.DesktopCoordinates.top;
		isValid = true;
		DXGI_ADAPTER_DESC Desc;
		pAdapter->GetDesc(&Desc);
		memory = static_cast<int>(Desc.DedicatedVideoMemory / 1024 / 1024);
		wcscpy_s(static_cast<wchar_t*>(cardName), 100, static_cast<wchar_t*>(Desc.Description));
		DXRelacher(pOutput);
		DXRelacher(pAdapter);
		DXRelacher(pFactory);
	}

	bool DeviceInfo::IsValid() const
	{
		return isValid;
	}

	int DeviceInfo::GetWidth() const
	{
		return width;
	}

	int DeviceInfo::GetHeight() const
	{
		return height;
	}

	int DeviceInfo::GetMemory() const
	{
		return memory;
	}

	wchar_t const* DeviceInfo::GetCardName() const
	{
		return static_cast<wchar_t const*>(cardName);
	}

	void DeviceInfo::GetDesc(DXGI_MODE_DESC& modeDesc)
	{
		modeDesc = mode;
	}
}
