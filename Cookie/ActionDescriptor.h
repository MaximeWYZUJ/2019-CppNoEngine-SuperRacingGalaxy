#pragma once

#include <chrono>
#include <functional>

#include "ExportMacro.h"
#include "InputManager.h"
#include "CompilerFlags.h"

namespace Cookie
{
	enum class StateType
	{
		Pressed,
		Released
	};

	class COOKIE_API ActionDescriptor
	{
	public:
		struct COOKIE_API Callbacks
		{
			using Action = std::function<void()>;

			Callbacks() = default;
			Callbacks(Action onBegin, Action onSuccess, Action onCancel)
				: onBegin(onBegin), onSuccess(onSuccess), onCancel(onCancel)
			{
			}
			
			Action onBegin;
			Action onSuccess;
			Action onCancel;
		};
		
		ActionDescriptor(Key key, StateType state, std::chrono::milliseconds const& delay, std::chrono::milliseconds const& repeatDelay, Callbacks callbacks);

		Key key;
		StateType state;
		std::chrono::milliseconds delay;
		std::chrono::milliseconds repeatDelay;
		Callbacks callbacks;

		friend struct std::hash<ActionDescriptor>;
		friend struct std::equal_to<ActionDescriptor>;
	};
}


template<>
struct std::hash<Cookie::ActionDescriptor>
{
	size_t operator()(Cookie::ActionDescriptor const& descriptor) const noexcept
	{
		if constexpr (Cookie::ArchWidth == Cookie::_32bits)
		{
			int32_t res = static_cast<int32_t>(descriptor.key) << 16;
			res |= static_cast<int16_t>(descriptor.state) << 8;
			res |= static_cast<int8_t>(descriptor.delay.count());
			return res;
		}
		else if constexpr (Cookie::ArchWidth == Cookie::_64bits)
		{
			int64_t res = static_cast<int64_t>(descriptor.key) << 32;
			res |= static_cast<int32_t>(descriptor.state) << 16;
			res |= static_cast<int16_t>(descriptor.delay.count());
			return res;
		}
	}
};

template<>
struct std::equal_to<Cookie::ActionDescriptor>
{
	bool operator()(Cookie::ActionDescriptor const& lhs, Cookie::ActionDescriptor const& rhs) const noexcept
	{
		return lhs.key == rhs.key &&
			lhs.state == rhs.state &&
			lhs.delay == rhs.delay;
	}
};