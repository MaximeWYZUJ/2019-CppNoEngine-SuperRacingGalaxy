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
		: device{device}, mouseCurrentPosition(0, 0), mousePreviousPosition(0, 0)
	{
		isInitialized = false;
		directInput = nullptr;
		keyboardInput = nullptr;
		joystickInput = nullptr;

		keyboardCurrentBuffer = keyboardBuffer1;
		keyboardPreviousBuffer = keyboardBuffer2;

		keyCurrentStates = keyStates1;
		keyPreviousStates = keyStates2;

		fill(begin(keyStates1), end(keyStates1), 0);
		fill(begin(keyStates2), end(keyStates2), 0);

		InitKeyMapping();
		Init();
	}

	InputManager::~InputManager()
	{
		if (keyboardInput)
		{
			keyboardInput->Unacquire();
			keyboardInput->Release();
			keyboardInput = nullptr;
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
		auto deviceEvents = device->GetEvents();
		for (auto e : deviceEvents)
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
			{
				auto const ev = get<MouseMoveData>(e.data);
				mouseCurrentPosition = ev.pos;
				break;
			}
			case DeviceEventType::MouseButton:
			{
				auto const ev = get<MouseButtonData>(e.data);

				if (ev.data == MouseButtonEventType::LeftButtonDown)
				{
					mouseCurrentPosition = ev.pos;
					mouseCurrentBuffer[static_cast<uint8_t>(MouseButton::LeftMouseButton)] = 0xFF;
				}
				else if (ev.data == MouseButtonEventType::LeftButtonUp)
				{
					mouseCurrentPosition = ev.pos;
					mouseCurrentBuffer[static_cast<uint8_t>(MouseButton::LeftMouseButton)] = 0x00;
				}

				break;
			}
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
				return;
			}
		}
		
		for (int i = 0; i < nbKeys; ++i)
		{
			keyCurrentStates[i] = IsKeyPressed(static_cast<Key>(i)) * numeric_limits<uint8_t>::max();

			if (keyCurrentStates[i] != keyPreviousStates[i])
			{
				events.push_back(InputEvent{
					.type = InputEventType::KeyStateChanged,
					.data = KeyStateChanged{ .key = static_cast<Key>(i), .position = keyCurrentStates[i] }
				});
			}
		}
	}

	void InputManager::PostUpdate()
	{
		swap(keyboardCurrentBuffer, keyboardPreviousBuffer);
		swap(keyCurrentStates, keyPreviousStates);
		copy(begin(mouseCurrentBuffer), end(mouseCurrentBuffer), begin(mousePreviousBuffer));
		mousePreviousPosition = mouseCurrentPosition;

		events.clear();
	}
	
	bool InputManager::IsKeyPressed(Key key)
	{
		return keyboardCurrentBuffer[keyToDirectXKey[static_cast<uint8_t>(key)]] & 0x80;
	}

	bool InputManager::IsMouseButtonPressed(MouseButton button)
	{
		return mouseCurrentBuffer[static_cast<uint8_t>(button)] & 0xFF;
	}

	Vector2<int> InputManager::GetMousePosition()
	{
		return mouseCurrentPosition;
	}

	Vector2<int> InputManager::GetMouseDelta()
	{
		return mouseCurrentPosition - mousePreviousPosition;
	}

	std::vector<InputEvent> const& InputManager::GetEvents() const
	{
		return events;
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
