#include <stdafx.h>
#include "SpriteFontLoader.h"
#include "../Helpers/BinaryReader.h"
#include "../Content/ContentManager.h"
#include "../Graphics/TextRenderer.h"
#include "../Graphics/TextureData.h"


SpriteFontLoader::SpriteFontLoader()
{

}


SpriteFontLoader::~SpriteFontLoader()
{
}

#pragma warning( disable : 4702 )
SpriteFont* SpriteFontLoader::LoadContent(const wstring& assetFile)
{
	auto pBinReader = new BinaryReader(); //Prevent memory leaks
	pBinReader->Open(assetFile);

	if (!pBinReader->Exists())
	{
		delete pBinReader;
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > Failed to read the assetFile!\nPath: \'%s\'", assetFile.c_str());
		
		return nullptr;
	}

	//See BMFont Documentation for Binary Layout
	
	//Parse the Identification bytes (B,M,F)
	{
		std::string input = "";
		input += pBinReader->Read<char>();
		input += pBinReader->Read<char>();
		input += pBinReader->Read<char>();

		if (input != "BMF")
		{
			Logger::LogError(L"Not a valid .fnt font");
			return nullptr;
		}
	}
	//If Identification bytes doesn't match B|M|F,
	//Log Error (SpriteFontLoader::LoadContent > Not a valid .fnt font) &
	//return nullptr
	//...

	//Parse the version (version 3 required)
	{
		int input = pBinReader->Read<char>();
		if (input != 3)
		{
			Logger::LogError(L"Only .fnt version 3 is supported!");
			return nullptr;
		}
	}
	//If version is < 3,
	//Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	//return nullptr
	//...

	//Valid .fnt file
	auto pSpriteFont = new SpriteFont();
	//SpriteFontLoader is a friend class of SpriteFont
	//That means you have access to its privates (pSpriteFont->m_FontName = ... is valid)

	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	{
		char id = pBinReader->Read<char>();
		int size = pBinReader->Read<int>();

		if (id == 1)
		{
			pSpriteFont->m_FontSize = pBinReader->Read<INT16>();
			pBinReader->MoveBufferPosition(12);

			char input;
			do
			{
				input = pBinReader->Read<char>();
				if (input != NULL) pSpriteFont->m_FontName += input;
			} while (input != NULL);
		}
		else
		{
			Logger::LogError(L"Unexpected block!");
		}
	}
	//Retrieve the FontSize (will be -25, BMF bug) [SpriteFont::m_FontSize]
	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	//Retrieve the FontName [SpriteFont::m_FontName]
	//...

	

	//**********
	// BLOCK 1 *
	//**********
	{
		char id = pBinReader->Read<char>();
		int size = pBinReader->Read<int>();

		if (id == 2)
		{
			pBinReader->MoveBufferPosition(4);
			pSpriteFont->m_TextureWidth = pBinReader->Read<UINT16>();
			pSpriteFont->m_TextureHeight = pBinReader->Read<UINT16>();
			UINT16 pageCount = pBinReader->Read<UINT16>();

			if (pageCount > 1)
			{
				Logger::LogError(L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed");
			}
			pBinReader->MoveBufferPosition(5);
		}
		else
		{
			Logger::LogError(L"Unexpected block!");
		}
	}

	//Retrieve the blockId and blockSize
	//Retrieve Texture Width & Height [SpriteFont::m_TextureWidth/m_TextureHeight]
	//Retrieve PageCount
	//> if pagecount > 1
	//> Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed)
	//Advance to Block2 (Move Reader)
	//...

	//**********
	// BLOCK 2 *
	//**********
	{
		char id = pBinReader->Read<char>();
		int size = pBinReader->Read<int>();

		if (id == 3)
		{
			std::wstring pageName;
			char input;
			do
			{
				input = pBinReader->Read<char>();
				if (input != NULL) pageName += input;
			} while (input != NULL);

			if (pageName.length() <= 0)
			{
				Logger::LogError(L"Unexpected block!");
			}

			size_t find = assetFile.rfind(L"/");
			std::wstring texturePath = assetFile.substr(0, find + 1);
			texturePath.append(pageName);

			pSpriteFont->m_pTexture = ContentManager::Load<TextureData>(texturePath);
		}
		else
		{
			Logger::LogError(L"Unexpected block!");
		}
	}
	//Retrieve the blockId and blockSize
	//Retrieve the PageName (store Local)
	//	> If PageName is empty
	//	> Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite [Empty])
	//>Retrieve texture filepath from the assetFile path
	//> (ex. c:/Example/somefont.fnt => c:/Example/) [Have a look at: wstring::rfind()]
	//>Use path and PageName to load the texture using the ContentManager [SpriteFont::m_pTexture]
	//> (ex. c:/Example/ + 'PageName' => c:/Example/somefont_0.png)


	//...
	
	//**********
	// BLOCK 3 *
	//**********

	{
		char id = pBinReader->Read<char>();
		int size = pBinReader->Read<int>();

		if (id == 4)
		{
			int charCount = size / pSpriteFont->m_FontSize;
			pSpriteFont->m_CharacterCount = charCount;
			UINT id;
			wchar_t wcharId;
			UINT16 x;
			UINT16 y;

			for (size_t i = 0; i < charCount; i++)
			{

				if (i == 30)
				{
					int test = 5;
					test += 5;
				}

				wcharId = id = pBinReader->Read<UINT>();
				if (!pSpriteFont->IsCharValid(wcharId)) Logger::LogWarning(L"Character invalid");
				FontMetric &spriteM = pSpriteFont->GetMetric(wcharId);
				spriteM.IsValid = true;
				spriteM.Character = id;
				x = pBinReader->Read<UINT16>();
				y = pBinReader->Read<UINT16>();
				spriteM.Width = pBinReader->Read<UINT16>();
				spriteM.Height = pBinReader->Read<UINT16>();
				spriteM.OffsetX = pBinReader->Read<INT16>();
				spriteM.OffsetY = pBinReader->Read<INT16>();
				spriteM.AdvanceX = pBinReader->Read<INT16>();
				spriteM.Page = pBinReader->Read<UINT8>();
				UINT8 temp = pBinReader->Read<UINT8>();
				switch (temp)
				{
				case 1:
					spriteM.Channel = 2;
						break;
				case 2:
					spriteM.Channel = 1;
						break;
				case 4:
					spriteM.Channel = 0;
						break;
				case 8:
					spriteM.Channel = 3;
						break;
				case 15:
					spriteM.Channel = 4;
					break;
				default:
					break;
				}
				spriteM.TexCoord.x = x / (float)pSpriteFont->m_TextureWidth;
				spriteM.TexCoord.y = y / (float)pSpriteFont->m_TextureHeight;
			}
		}
		else
		{
			Logger::LogError(L"Unexpected block!");
		}
	}
	//Retrieve the blockId and blockSize
	//Retrieve Character Count (see documentation)
	//Retrieve Every Character, For every Character:
	//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
	//> Check if CharacterId is valid (SpriteFont::IsCharValid), Log Warning and advance to next character if not valid
	//> Retrieve the corresponding FontMetric (SpriteFont::GetMetric) [REFERENCE!!!]
	//> Set IsValid to true [FontMetric::IsValid]
	//> Set Character (CharacterId) [FontMetric::Character]
	//> Retrieve Xposition (store Local)
	//> Retrieve Yposition (store Local)
	//> Retrieve & Set Width [FontMetric::Width]
	//> Retrieve & Set Height [FontMetric::Height]
	//> Retrieve & Set OffsetX [FontMetric::OffsetX]
	//> Retrieve & Set OffsetY [FontMetric::OffsetY]
	//> Retrieve & Set AdvanceX [FontMetric::AdvanceX]
	//> Retrieve & Set Page [FontMetric::Page]
	//> Retrieve Channel (BITFIELD!!!) 
	//	> See documentation for BitField meaning [FontMetrix::Channel]
	//> Calculate Texture Coordinates using Xposition, Yposition, TextureWidth & TextureHeight [FontMetric::TexCoord]
	
	//...
	
	
	//DONE :)
	delete pBinReader;
	return pSpriteFont;
	
	#pragma warning(default:4702)  
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}