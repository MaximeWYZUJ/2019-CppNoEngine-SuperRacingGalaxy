#pragma once
#include <vector>
#include "TextureManager.h"
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

namespace Cookie
{
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
		
		TextureManager* textureManager;
		DeviceD3D11* device;
		ID3D11Buffer* pVertexBuffer;
		ID3D11VertexShader* pVertexShader;
		ID3D11InputLayout* pVertexLayout;
		ID3D11PixelShader* pPixelShader;
		ID3D11SamplerState* pSamplerState;
		ID3D11Buffer* pConstantBuffer;
		
		static SommetSprite sommets[6];
		
		std::vector<Sprite*> sprites;
		
		void InitShaders();
		static void InitText();
		static void CloseText();

		void SetPosDim(Sprite* sprite, int x_, int y_, int dx_, int dy_);
		
	public:
		uint32_t ScreenHeight;
		uint32_t ScreenWidth;
		
		GuiManager(TextureManager* textureManager, DeviceD3D11* device);
		~GuiManager();
		
		void newSprite(const std::string& textureName, int xPos, int yPos, int xDim = 1, int yDim = 1);
		Text* newText(int largeur, int hauteur, Gdiplus::Font* pFont, const std::wstring& s, int xPos, int yPos);
		
		void Write(const std::wstring& s, Text* text);

		void DrawAll();
	};
}


