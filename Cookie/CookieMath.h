#pragma once
#include "ExportMacro.h"

namespace Cookie
{
    namespace Math
    {
        constexpr double Pi = 3.14159265358979323846;

		bool COOKIE_API Approximately(float a, float b);
    	
		template<class T, class U>
		static auto Lerp(T a, T b, U f)
		{
			return a + static_cast<float>(f)* (b - a);
		}
    }
}