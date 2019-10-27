#include "pch.h"
#include <iostream>
#include <dinput.h>
#include "InputManager.h"
#include "Util.h"
#include "resource.h"
#include "DeviceD3D11.h"

namespace Cookie
{
	using namespace std;

	InputManager::InputManager(DeviceD3D11* device)
		: device{device}
	{
		isInitialized = false;
		directInput = nullptr;
		keyboardInput = nullptr;
		mouseInput = nullptr;
		joystickInput = nullptr;

		keyboardCurrentBuffer = keyboardBuffer1;
		keyboardPreviousBuffer = keyboardBuffer2;

		InitKeyMapping();
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

	bool InputManager::Init()
	{
		if (!isInitialized)
		{
			DXEssayer(DirectInput8Create(device->GetModule(),
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
			keyboardInput->SetCooperativeLevel(device->GetWindow(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

			isInitialized = true;
		}
		return true;
	}
	
	void InputManager::Update()
	{
		auto events = device->GetEvents();
		for (auto e : events)
		{
			switch (e.type)
			{
			case DeviceEventType::Focus:
				keyboardInput->Acquire();
				break;
			case DeviceEventType::FocusLost:
				keyboardInput->Unacquire();
				break;
			case DeviceEventType::MouseMove:
				auto const ev = e.As<MouseMove>();
				mouseCurrentPosition = ev.data->pos;
				break;
			default:
				break;
			}
		}

		if (device->HasFocus())
		{
			HRESULT const res = keyboardInput->GetDeviceState(sizeof keyboardBuffer1, static_cast<void*>(keyboardCurrentBuffer));

			if (res != DI_OK)
			{
				std::cout << "Unable to get keyboard state!" << std::endl;
			}
		}
	}

	void InputManager::PostUpdate()
	{
		swap(keyboardCurrentBuffer, keyboardPreviousBuffer);
		copy(begin(mouseCurrentBuffer), end(mouseCurrentBuffer), begin(mousePreviousBuffer));
		mousePreviousPosition = mouseCurrentPosition;
	}
	
	bool InputManager::IsKeyPressed(Key key)
	{
		return keyboardCurrentBuffer[keyToDirectXKey[static_cast<uint8_t>(key)]] & 0x80;
	}

	void InputManager::InitKeyMapping()
	{
		keyToDirectXKey[static_cast<int>(Key::A)] = DIK_A;
		keyToDirectXKey[static_cast<int>(Key::B)] = DIK_B;
		keyToDirectXKey[static_cast<int>(Key::C)] = DIK_C;
		keyToDirectXKey[static_cast<int>(Key::D)] = DIK_D;
		keyToDirectXKey[static_cast<int>(Key::E)] = DIK_E;
		keyToDirectXKey[static_cast<int>(Key::F)] = DIK_F;
		keyToDirectXKey[static_cast<int>(Key::G)] = DIK_G;
		keyToDirectXKey[static_cast<int>(Key::H)] = DIK_H;
		keyToDirectXKey[static_cast<int>(Key::I)] = DIK_I;
		keyToDirectXKey[static_cast<int>(Key::J)] = DIK_J;
		keyToDirectXKey[static_cast<int>(Key::K)] = DIK_K;
		keyToDirectXKey[static_cast<int>(Key::L)] = DIK_L;
		keyToDirectXKey[static_cast<int>(Key::M)] = DIK_M;
		keyToDirectXKey[static_cast<int>(Key::N)] = DIK_N;
		keyToDirectXKey[static_cast<int>(Key::O)] = DIK_O;
		keyToDirectXKey[static_cast<int>(Key::P)] = DIK_P;
		keyToDirectXKey[static_cast<int>(Key::Q)] = DIK_Q;
		keyToDirectXKey[static_cast<int>(Key::R)] = DIK_R;
		keyToDirectXKey[static_cast<int>(Key::S)] = DIK_S;
		keyToDirectXKey[static_cast<int>(Key::T)] = DIK_T;
		keyToDirectXKey[static_cast<int>(Key::U)] = DIK_U;
		keyToDirectXKey[static_cast<int>(Key::V)] = DIK_V;
		keyToDirectXKey[static_cast<int>(Key::W)] = DIK_W;
		keyToDirectXKey[static_cast<int>(Key::X)] = DIK_X;
		keyToDirectXKey[static_cast<int>(Key::Y)] = DIK_Y;
		keyToDirectXKey[static_cast<int>(Key::Z)] = DIK_Z;
	}
}
