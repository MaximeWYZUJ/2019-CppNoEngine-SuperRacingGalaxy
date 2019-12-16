#pragma once
#include <vector>
#include "TextureManager.h"
#include <gdiplus.h>
#include "Shaders.h"
#include "Button.h"

#pragma comment(lib, "gdiplus.lib")

namespace Cookie
{
	class InputManager;
	struct Text;
	class Sprite;

	class SommetSprite
	{
	public:
		SommetSprite() = default;
		SommetSprite(const Vector3<>& position, const Vector2<>& coordTex) : m_Position(position), m_CoordTex(coordTex)
		{

		}
		static UINT numElements;
		static D3D11_INPUT_ELEMENT_DESC layout[];
		Vector3<> m_Position;
		Vector2<> m_CoordTex;
	};
	
	class COOKIE_API GuiManager
	{		
		// Variables statiques pour GDI+
		static ULONG_PTR token;

		Shaders shader;
		
		TextureManager* textureManager;
		InputManager* inputManager;
		DeviceD3D11* device;
		ID3D11Buffer* pVertexBuffer;
		
		static SommetSprite sommets[6];
		
		std::vector<Sprite*> sprites;
		std::vector<Button*> buttons;
		
		static void InitText();
		static void CloseText();

		void SetPosDim(Sprite* sprite, int x_, int y_, int dx_, int dy_, int xDimPix, int yDimPix) const;
	public:
		uint32_t ScreenHeight;
		uint32_t ScreenWidth;
		
		GuiManager(TextureManager* textureManager, InputManager* inputManager, DeviceD3D11* device);
		~GuiManager();
		
		Sprite* newSprite(const std::string& textureName, int xPos, int yPos, int xScale = 1, int yScale = 1, int xDimPix = 0, int yDimPix = 0);
		Text* newText(int width, int height, Gdiplus::Font* font, const std::wstring& text_, int xPos, int yPos);
		Button* newButton(const std::string& textureBackgroung, const std::string& textureOver, int width, int height, Gdiplus::Font* pFont, const std::wstring& text_, int xPos, int yPos, std::function<void()> action = []() {}, int offsetTextBoutonX = 0, int offsetTextBoutony = 0);
		void deleteGuiElement(Sprite* sprite);

		void changeSpriteTexture(const std::string& textureName, Sprite* sprite) const;
		void changeButtonBackground(const std::string& textureName, Button* button) const;
		
		void Write(const std::wstring& s, Text* text) const;
		
		void Update();
		void DrawAll();
	};
}


