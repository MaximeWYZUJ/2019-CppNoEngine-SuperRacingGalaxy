#pragma once

#include <algorithm>

namespace Cookie
{
	namespace Alg
	{
		template<class TContainer, class TVal>
		typename TContainer::iterator Find(TContainer const& container, TVal const& val)
		{
			return std::find(std::begin(container), std::end(container), val);
		}

		template<class TContainer, class TVal>
		bool Exists(TContainer const& container, TVal const& val)
		{
			return std::find(std::begin(container), std::end(container), val) != std::end(container);
		}
	}
}