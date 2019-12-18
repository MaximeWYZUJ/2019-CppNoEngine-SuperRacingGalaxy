#include "pch.h"
#include "CookieMath.h"
#include <cmath>
#include <algorithm>

#undef max

namespace Cookie
{
	bool Math::Approximately(float a, float b)
	{
		// From Unity3D, may not be correct for C++ code...

				// If a or b is zero, compare that the other is less or equal to epsilon.
				// If neither a or b are 0, then find an epsilon that is good for
				// comparing numbers at the maximum magnitude of a and b.
				// Floating points have about 7 significant digits, so
				// 1.000001f can be represented while 1.0000001f is rounded to zero,
				// thus we could use an epsilon of 0.000001f for comparing values close to 1.
				// We multiply this epsilon by the biggest magnitude of a and b.
		return abs(b - a) < std::max(0.000001f * std::max(abs(a), abs(b)), 1e-38f * 8);
	}

	float Math::Sign(float a)
	{
		return (a >= 0.0f) ? +1.0f : -1.0f;
	}
}
