#pragma once
#include <chrono>

namespace Cookie
{
	struct KeyStateChanged;
	class ActionDescriptor;
	
	class ActionRunner
	{
	public:
		using Action = void(*)();
		
		ActionRunner(ActionDescriptor const* descriptor);

		void Update();
		void Run(KeyStateChanged const& keyState);
	private:
		[[nodiscard]]
		bool StateMatch(KeyStateChanged const& keyState) const;
		
		[[nodiscard]]
		bool StateReversed(KeyStateChanged const& keyState) const;
		
		ActionDescriptor const* descriptor;
		
		bool isRunning;
		bool isRepeating;
		std::chrono::steady_clock::time_point runStart;
		std::chrono::steady_clock::time_point lastSuccess;
	};
}
