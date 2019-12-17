#pragma once

#include "ExportMacro.h"

#include <dinput.h>
#include "Vector2.h"
#include <vector>
#include <variant>

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
		Alpha0 = 0x1A,
		Alpha1 = 0x1B,
		Alpha2 = 0x1C,
		Alpha3 = 0x1D,
		Alpha4 = 0x1E,
		Alpha5 = 0x1F,
		Alpha6 = 0x20,
		Alpha7 = 0x21,
		Alpha8 = 0x22,
		Alpha9 = 0x23,
		Unknown = 0x24
	};

	enum class Mouse
	{
		LeftButton = 0x00,
		RightButton = 0x01
	};

	// Device with x/y coordinates
	enum class PositionalDevice
	{
		Mouse
	};

	enum class InputEventType
	{
		KeyStateChanged,
		MouseStateChanged,
		PositionChanged
	};

	struct KeyStateChanged
	{
		Key key;
		
		// 0 = initial state, 255 = fully pressed
		uint8_t position;
	};

	struct MouseStateChanged
	{
		Mouse button;

		// 0 = initial state, 255 = fully pressed
		uint8_t position;
	};

	struct PositionChanged
	{
		PositionalDevice device;

		Vector2<int> pos;
	};

	struct InputEvent
	{
		using DataType = std::variant<KeyStateChanged, MouseStateChanged, PositionChanged>;
		
		InputEventType type;
		DataType data;
	};
	
	class COOKIE_API InputManager
	{
	public:
		InputManager(DeviceD3D11* device);
		~InputManager();

		bool Init();
		void Update();
		void PostUpdate();

		/// <summary>
		///		Indicate if a key has been pressed during this frame.
		///	</summary>
		bool GetKeyDown(Key key) const noexcept;

		/// <summary>
		///		Indicate if a key is currently pressed. Return true as long as the key is pressed.
		///	</summary>
		bool IsKeyPressed(Key key) const noexcept;
		bool IsMouseButtonPressed(Mouse button);
		Vector2<int> GetMousePosition();
		Vector2<int> GetMouseDelta();
		float GetMouseWheelRotation() const noexcept;
		float GetMouseWheelDelta() const noexcept;

		[[nodiscard]]
		std::vector<InputEvent> const& GetEvents() const;
	private:
		void InitKeyMapping();
		
		static constexpr int nbKeys = static_cast<int>(Key::Unknown);

		DeviceD3D11* device;
		bool isInitialized;
		
		IDirectInput8* directInput;
		IDirectInputDevice8* keyboardInput;
		IDirectInputDevice8* joystickInput;

		uint8_t* keyboardCurrentBuffer;
		uint8_t* keyboardPreviousBuffer;
		uint8_t keyboardBuffer1[256];
		uint8_t keyboardBuffer2[256];
		uint8_t keyToDirectXKey[256];

		uint8_t* keyCurrentStates;
		uint8_t* keyPreviousStates;
		uint8_t keyStates1[nbKeys];
		uint8_t keyStates2[nbKeys];

		uint8_t mouseCurrentBuffer[16];
		uint8_t mousePreviousBuffer[16];

		Vector2<int> mouseCurrentPosition;
		Vector2<int> mousePreviousPosition;

		float mouseWheelCurRotation;
		float mouseWheelDelta;

		std::vector<InputEvent> events;
	};
}
