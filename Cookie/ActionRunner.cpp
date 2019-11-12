#include "pch.h"
#include "ActionRunner.h"
#include <chrono>
#include "ActionDescriptor.h"

namespace Cookie
{
	using namespace std;
	
	ActionRunner::ActionRunner(ActionDescriptor const* descriptor)
		: descriptor(descriptor), isRunning(false)
	{
	}
	
	void ActionRunner::Update()
	{
		if (isRunning)
		{
			auto currentTime = chrono::high_resolution_clock::now();
			if (currentTime - runStart >= descriptor->delay)
			{
				isRunning = false;
				descriptor->callbacks.onSuccess();
			}
		}
	}

	void ActionRunner::Run(KeyStateChanged const& keyState)
	{
		if (keyState.key == descriptor->key)
		{
			if (isRunning)
			{
				if (StateReversed(keyState))
				{
					isRunning = false;
					descriptor->callbacks.onCancel();
					return;
				}
				
				auto currentTime = chrono::high_resolution_clock::now();
				if (currentTime - runStart >= descriptor->delay)
				{
					isRunning = false;
					descriptor->callbacks.onSuccess();
				}
			}
			else if (StateMatch(keyState))
			{
				runStart = chrono::high_resolution_clock::now();
				isRunning = true;
				descriptor->callbacks.onBegin();
				Run(keyState);
			}
		}
	}
	
	bool ActionRunner::StateMatch(KeyStateChanged const& keyState) const
	{
		return keyState.position == 255 && descriptor->state == StateType::Pressed ||
			keyState.position == 0 && descriptor->state == StateType::Released;
	}
	
	bool ActionRunner::StateReversed(KeyStateChanged const& keyState) const
	{
		return keyState.position == 255 && descriptor->state == StateType::Released ||
			keyState.position == 0 && descriptor->state == StateType::Pressed;
	}
}
