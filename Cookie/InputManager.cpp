#include "pch.h"
#include <iostream>
#include <dinput.h>
#include "InputManager.h"
#include "Util.h"
#include "resource.h"
#include "Device.h"
#include "DeviceD3D11.h"

namespace Cookie
{
	bool InputManager::isInitialized = false;
	
	InputManager::InputManager()
	{
		directInput = nullptr;
		keyboardInput = nullptr;
		mouseInput = nullptr;
		joystickInput = nullptr;
	}

	InputManager::~InputManager()
	{
		if (keyboardInput)
		{
			keyboardInput->Unacquire();
			keyboardInput->Release();
			keyboardInput = nullptr;
		}
		if (mouseInput)
		{
			mouseInput->Unacquire();
			mouseInput->Release();
			mouseInput = nullptr;
		}
		if (joystickInput)
		{
			joystickInput->Unacquire();
			joystickInput->Release();
			joystickInput = nullptr;
		}
		if (directInput)
		{
			directInput->Release();
			directInput = nullptr;
		}

	}

	bool InputManager::Init(HINSTANCE hInstance, Device* device)
	{
		if (!isInitialized)
		{
			DXEssayer(DirectInput8Create(hInstance,
				DIRECTINPUT_VERSION,
				IID_IDirectInput8,
				(void**)&directInput,
				NULL), DIRECTINPUT_CREATION_ERROR);
			
			// Keyboard
			DXEssayer(directInput->CreateDevice(GUID_SysKeyboard,
				&keyboardInput,
				NULL),
				KEYBOARD_CREATION_ERROR);
			DXEssayer(keyboardInput->SetDataFormat(&c_dfDIKeyboard), KEYBOARD_FORMAT_CREATION_ERROR);
			// Todo: Generic service to get window handle
			keyboardInput->SetCooperativeLevel(dynamic_cast<DeviceD3D11*>(device)->GetWindowHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			keyboardInput->Acquire();

			isInitialized = true;
		}
		return true;
	}
	
	void InputManager::UpdateKeyboardState()
	{
		HRESULT res = keyboardInput->GetDeviceState(sizeof(keyboardBuffer), static_cast<void*>(&keyboardBuffer));

		if (res != DI_OK)
		{
			std::cout << "problem" << std::endl;
		}
	}
	
	bool InputManager::IsKeyPressed(Key keyCode)
	{
		return (keyboardBuffer[static_cast<uint8_t>(keyCode)] & 0x80);
	}
}
