//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ControlsScreen.h"
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

ControlsScreen::ControlsScreen(void) :
GameScene(L"ControlsScreen")
{
}


ControlsScreen::~ControlsScreen(void)
{
}

void ControlsScreen::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto obj = new GameObject();
	obj->AddComponent(new SpriteComponent(L"./Resources/Sprites/BMAN_Controlscheme.png", XMFLOAT2(0,0),XMFLOAT4(1,1,1,1)));
	AddChild(obj);
	obj->GetTransform()->Translate(0, 0, .91f);
}

void ControlsScreen::Update(const GameContext& gameContext)
{
	if (!gameContext.pInput->IsMouseButtonDown(1, true) && gameContext.pInput->IsMouseButtonDown(1))
	{
		SceneManager::GetInstance()->SetActiveGameScene(L"BombermanLevel");
	}
}

void ControlsScreen::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}