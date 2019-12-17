#pragma once
#include "Matrix4x4.h"

namespace Cookie
{
	struct COOKIE_API Sprite
	{
		ID3D11ShaderResourceView* pTextureD3D;
		ID3D11ShaderResourceView* pTextureSwap;
		
		enum class RotType
		{
			staticRot,
			randomRot,
			continuousRot
		};

		RotType rotationType;

		Vector3<> axe;
		
		float rotAngleMin;
		float rotAngleMax;
		float actualRot;

		Matrix4x4<> Translation;
		Matrix4x4<> Scale;
		
		Matrix4x4<> matPosDim;
		Sprite() : pTextureD3D{ nullptr }, pTextureSwap{ nullptr }, rotationType{ RotType::staticRot } {}

		void Swap()
		{
			if (pTextureSwap != nullptr)
				std::swap(pTextureD3D, pTextureSwap);
		}
	};
}

