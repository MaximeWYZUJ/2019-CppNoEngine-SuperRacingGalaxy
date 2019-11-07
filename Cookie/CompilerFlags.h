#pragma once

namespace Cookie
{
#ifdef _WIN64
		constexpr int ArchWidth = 64;
#elif _WIN32
		constexpr int ArchWidth = 32;
#endif
		constexpr int _32bits = 32;
		constexpr int _64bits = 64;
}
