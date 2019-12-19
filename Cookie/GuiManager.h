#pragma once
#include <vector>
#include "TextureManager.h"
#include <gdiplus.h>
#include "Shaders.h"
#include "Button.h"
#include <random>

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

		int frameNumber;
		std::mt19937 prng;
		
		static void InitText();
		static void CloseText();

		void SetPosDim(Sprite* sprite, int x_, int y_, int dx_, int dy_, int xDimPix, int yDimPix, Matrix4x4<> rotMat) const;
		void ActualizeRot(Sprite* sprite);
	public:
		uint32_t ScreenHeight;
		uint32_t ScreenWidth;
		
		GuiManager(TextureManager* textureManager, InputManager* inputManager, DeviceD3D11* device);
		~GuiManager();
		
		Sprite* newSprite(const std::string& textureName, int xPos, int yPos, int xScale = 1, int yScale = 1, int xDimPix = 0, int yDimPix = 0, float angleRotMin = 0.0f, Vector3<> axeRot = {}, bool rotStatic = true, float angleRotMax = 0.0f);
		Text* newText(int width, int height, Gdiplus::Font* font, const std::wstring& text_, int xPos, int yPos, Gdiplus::Color color = Gdiplus::Color(255, 255, 255, 255), float angleRotMin = 0.0f, Vector3<> axeRot = {}, bool rotStatic = true, float angleRotMax = 0.0f);
		Button* newButton(const std::string& textureBackgroung, const std::string& textureOver, int width, int height, Gdiplus::Font* pFont, const std::wstring& text_, int xPos, int yPos, std::function<void()> action = []() {}, Gdiplus::Color color = Gdiplus::Color(255, 255, 255, 255), int offsetTextBoutonX = 0, int offsetTextBoutony = 0, float angleRotMin = 0.0f, Vector3<> axeRot = {}, bool rotStatic = true, float angleRotMax = 0.0f);
		void deleteGuiElement(Sprite* sprite);

		void addSwapTextureSprite(const std::string& textureName, Sprite* sprite, bool specialPredicate = false, std::function<bool()> predicate = []() {return true; }) const;
		void changeSpriteTexture(const std::string& textureName, Sprite* sprite) const;
		void changeButtonBackground(const std::string& textureName, Button* button) const;
		void changeTextColor(Text* text, const Gdiplus::Color color);
		void changeTextFont(Text* text, Gdiplus::Font* font);
		
		void Write(const std::wstring& s, Text* text) const;
		
		void Update();
		void DrawAll();
	};
}


