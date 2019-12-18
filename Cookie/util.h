#pragma once
#include <string>

namespace Cookie
{
	template <class Type>
	inline void DXEssayer(const Type& Resultat)
	{
		if (Resultat != S_OK)
		{
			throw Resultat;
		}
	}

	template <class Type1, class Type2>
	inline void DXEssayer(const Type1& Resultat, const Type2& unCode)
	{
		if (Resultat != S_OK)
		{
			throw unCode;
		}
	}

	template <class Type>
	inline void DXValider(const void* UnPointeur, const Type& unCode)
	{
		if (UnPointeur == nullptr)
		{
			throw unCode;
		}
	}

	template <class Type>
	inline void DXRelacher(Type& UnPointeur)
	{
		if (UnPointeur != nullptr)
		{
			UnPointeur->Release();
			UnPointeur = nullptr;
		}
	}

	template <class _Container>
	class StackInsertIterator
	{
	public:
		using value_type = void;
		using difference_type = void;
		using pointer = void;
		using reference = void;

		using container_type = _Container;

		explicit StackInsertIterator(_Container& _Cont) noexcept : container(_STD addressof(_Cont)) {}

		StackInsertIterator& operator=(const typename _Container::value_type& _Val)
		{
			container->push(_Val);
			return *this;
		}

		StackInsertIterator& operator=(typename _Container::value_type&& _Val)
		{
			container->push(_STD move(_Val));
			return *this;
		}

	protected:
		_Container* container; // pointer to container
	};

	template <class _Container>
	_NODISCARD StackInsertIterator<_Container> StackInserter(_Container& _Cont) noexcept
	{
		return StackInsertIterator<_Container>(_Cont);
	}

	inline std::wstring ToWide(std::string const& s)
	{
		return std::wstring(begin(s), end(s));
	}


	inline void ThrowOnError(HRESULT result)
	{
		if (FAILED(result))
		{
			throw std::exception("Direct3D ThrowOnError");
		}
	}
}