#pragma once

namespace SmallEngine
{
	class DeviceInfo
	{
	public:
		enum DEVICEINFO_TYPE
		{
			CURRENT_ADAPTER
		};

		DeviceInfo(DXGI_MODE_DESC modeDesc);
		explicit DeviceInfo(int adapterNum);

		bool IsValid() const;
		int GetWidth() const;
		int GetHeight() const;
		int GetMemory() const;
		wchar_t const* GetCardName() const;
		void GetDesc(DXGI_MODE_DESC& modeDesc);
	private:
		bool isValid;
		int width;
		int height;
		int memory;
		wchar_t cardName[100];
		DXGI_MODE_DESC mode;
	};
}