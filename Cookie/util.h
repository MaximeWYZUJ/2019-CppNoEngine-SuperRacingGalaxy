#pragma once

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
}