//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "MainMenu.h"
#include "Scenegraph\GameObject.h"
#include "Scenegraph\SceneManager.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"
#include "Graphics\SpriteRenderer.h"

#include "Content\ContentManager.h"
#include "Graphics\TextureData.h"
#include "Components\SpriteComponent.h"
#include "Components\TransformComponent.h"
#include "Graphics\MeshFilter.h"
#include "Base/OverlordGame.h"
#define FPS_COUNTER 1

MainMenu::MainMenu(void) :
GameScene(L"MainMenu"),
m_FpsInterval(FPS_COUNTER)
{
}


MainMenu::~MainMenu(void)
{
}

void MainMenu::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto obj = new GameObject();
	obj->AddComponent(new SpriteComponent(L"./Resources/Sprites/BMAN_Menu_Background.png", XMFLOAT2(0,0),XMFLOAT4(1,1,1,1)));
	AddChild(obj);
	obj->GetTransform()->Translate(0, 0, .91f);

	float width, height;
	width = OverlordGame::GetGameSettings().Window.Width;
	height = OverlordGame::GetGameSettings().Window.Height;


	GameObject* menuItem = new GameObject();
	m_StartButton = new SpriteComponent(L"./Resources/Sprites/BMAN_Menu_Start.png", XMFLOAT2(0.5f, 0.5f), XMFLOAT4(1, 1, 1, 1));
	menuItem->AddComponent(m_StartButton);
	AddChild(menuItem);
	menuItem->GetTransform()->Translate(width * 0.5f, height * 0.5f - 75, .9f);

	menuItem = new GameObject();
	m_ExitButton = new SpriteComponent(L"./Resources/Sprites/BMAN_Menu_Exit.png", XMFLOAT2(0.5f, 0.5f), XMFLOAT4(1, 1, 1, 1));
	menuItem->AddComponent(m_ExitButton);
	AddChild(menuItem);
	menuItem->GetTransform()->Translate(width * 0.5f, height * 0.5f + 75, .9f);
}

void MainMenu::Update(const GameContext& gameContext)
{
	float width, height;
	width = OverlordGame::GetGameSettings().Window.Width;
	height = OverlordGame::GetGameSettings().Window.Height;
	POINT mousePos = gameContext.pInput->GetMousePosition();
	POINT startMenuPos; 
	startMenuPos.x = width * 0.5f;
	startMenuPos.y = height * 0.5f - 75;

	POINT exitMenuPos;
	exitMenuPos.x = width * 0.5f;
	exitMenuPos.y = height * 0.5f + 75;

	if (abs(mousePos.x - startMenuPos.x) < 180 && abs(mousePos.y - startMenuPos.y) < 50)
	{
		if (gameContext.pInput->IsMouseButtonDown(1))
		{
			SceneManager::GetInstance()->SetActiveGameScene(L"ControlsScreen");
		}

		if (!m_StartSelected) {
			m_StartSelected = true;
			m_StartButton->SetTexture(L"./Resources/Sprites/BMAN_Menu_Start_Selected.png");
		}
	}
	else 
	{
		if (m_StartSelected)
		{
			m_StartSelected = false;
			m_StartButton->SetTexture(L"./Resources/Sprites/BMAN_Menu_Start.png");
		}
	}

	if (abs(mousePos.x - exitMenuPos.x) < 180 && abs(mousePos.y - exitMenuPos.y) < 50)
	{
		if (gameContext.pInput->IsMouseButtonDown(1))
		{
			OverlordGame::Quit();
		}

		if (!m_ExitSelected) {
			m_ExitSelected = true;
			m_ExitButton->SetTexture(L"./Resources/Sprites/BMAN_Menu_Exit_Selected.png");
		}
	}
	else
	{
		if (m_ExitSelected)
		{
			m_ExitSelected = false;
			m_ExitButton->SetTexture(L"./Resources/Sprites/BMAN_Menu_Exit.png");
		}
	}

}

void MainMenu::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}