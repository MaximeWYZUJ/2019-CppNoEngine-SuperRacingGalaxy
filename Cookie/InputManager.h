#pragma once

#include <dinput.h>

namespace Cookie
{
	class Device;

	enum class Key
	{
		W = 0x11
	};
	
	class InputManager
	{
	public:
		InputManager();
		~InputManager();
		bool Init(HINSTANCE hInstance, Device* device);
		void UpdateKeyboardState();
		bool IsKeyPressed(Key keyCode);
	private:
		static bool isInitialized;
		
		IDirectInput8* directInput;
		IDirectInputDevice8* keyboardInput;
		IDirectInputDevice8* mouseInput;
		IDirectInputDevice8* joystickInput;
		
		uint8_t keyboardBuffer[256];
	};
}