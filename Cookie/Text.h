#pragma once
#include "Sprite.h"

namespace Cookie
{
	struct COOKIE_API Text : Sprite
	{
		UINT TextWidth;
		UINT TextHeight;

		class Gdiplus::Font* pFont;
		Gdiplus::Bitmap* pCharBitmap;
		Gdiplus::Graphics* pCharGraphics;
		Gdiplus::SolidBrush* pBrush;
		ID3D11Texture2D* pTexture;
	};	
}

