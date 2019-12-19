#pragma once
#include "Matrix4x4.h"
#include <functional>

namespace Cookie
{
	struct COOKIE_API Sprite
	{
		
		enum class RotType
		{
			staticRot,
			randomRot,
			continuousRot
		};

		ID3D11ShaderResourceView* pTextureD3D;
		ID3D11ShaderResourceView* pTextureSwap;

		std::function<bool()> predicate;
		bool specialPredicate = false;
		
		RotType rotationType;

		Vector3<> axe;
		
		float rotAngleMin;
		float rotAngleMax;
		float actualRot;

		Matrix4x4<> Translation;
		Matrix4x4<> Scale;
		Matrix4x4<> ScaleScreen;
		
		Matrix4x4<> matPosDim;
		Sprite() : pTextureD3D{ nullptr }, pTextureSwap{ nullptr }, rotationType{ RotType::staticRot } {}

		void Swap()
		{
			if (pTextureSwap != nullptr)
				std::swap(pTextureD3D, pTextureSwap);
		}
	};
}

