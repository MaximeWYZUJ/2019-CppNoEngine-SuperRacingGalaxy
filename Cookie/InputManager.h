#pragma once

#include <dinput.h>

namespace Cookie
{
	class Device;
	class DeviceD3D11;

	enum class Key
	{
		A = 0x00,
		B = 0x01,
		C = 0x02,
		D = 0x03,
		E = 0x04,
		F = 0x05,
		G = 0x06,
		H = 0x07,
		I = 0x08,
		J = 0x09,
		K = 0x0A,
		L = 0x0B,
		M = 0x0C,
		N = 0x0D,
		O = 0x0E,
		P = 0x0F,
		Q = 0x10,
		R = 0x11,
		S = 0x12,
		T = 0x13,
		U = 0x14,
		V = 0x15,
		W = 0x16,
		X = 0x17,
		Y = 0x18,
		Z = 0x19,
	};
	
	class InputManager
	{
	public:
		InputManager(DeviceD3D11* device);
		~InputManager();

		bool Init();
		void Update();
		bool IsKeyPressed(Key key);
	private:
		void InitKeyMapping();

		DeviceD3D11* device;
		bool isInitialized;
		
		IDirectInput8* directInput;
		IDirectInputDevice8* keyboardInput;
		IDirectInputDevice8* mouseInput;
		IDirectInputDevice8* joystickInput;
		
		uint8_t keyboardBuffer[256];
		uint8_t keyToDirectXKey[256];
	};
}
