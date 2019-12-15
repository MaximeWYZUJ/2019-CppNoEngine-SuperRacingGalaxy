#pragma once
#include "Sprite.h"
#include "Text.h"
#include <functional>

namespace Cookie
{
	struct COOKIE_API Button : Sprite
	{
		int xMin;
		int xMax;
		int yMin;
		int yMax;

		bool over = false;
		
		std::function<void()> clickAction;
		Sprite* background;
		Text* text;
		ID3D11ShaderResourceView* pTextureOver;

		void Click()
		{
			clickAction();
		}

		void MouseOver()
		{
			std::swap(background->pTextureD3D, pTextureOver);
		}

		void OnClick(std::function<void()> action)
		{
			clickAction = action;
		}

	};
}
