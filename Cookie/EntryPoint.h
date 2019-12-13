#pragma once

#include "ExportMacro.h"
#include "Engine.h"

namespace Cookie
{
	class COOKIE_API EntryPoint
	{
	public:
		static std::unique_ptr<Engine> CreateStandaloneEngine();
	};
}