//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "Base\stdafx.h"

#include "BombermanLevel.h"
#include "Scenegraph\GameObject.h"
#include "Components\TransformComponent.h"
#include "../OverlordProject/Materials/ColorMaterial.h""
#include <Diagnostics/DebugRenderer.h>
#include "Prefabs\Prefabs.h"
#include "PlayingField.h"
#include "Player.h"
#include "../OverlordProject/Materials/SkinnedDiffuseMaterial.h"
#include "../OverlordProject/Materials/DiffuseMaterial.h"
#include "../OverlordProject/Materials/FoamMaterial.h"
#include "../OverlordProject/Materials/WaterMaterial.h"
#include "Components/SpriteComponent.h"
#include "Base/OverlordGame.h"
#include "../OverlordProject/Materials/Post\PostGrayscale.h"
#include "../OverlordProject/Materials/Post\PostKuwahara.h"
#include "../OverlordProject/Materials/Post\PostKuwaharaCharcoal.h"
#include "../OverlordProject/Materials/Post\PostBarrel.h"
#include "../OverlordProject/Materials/Post\PostCircles.h"
#include "../OverlordProject/Materials/Post\PostEdges.h"
#include "../OverlordProject/Materials/Post\PostMonoScanline.h"
#include "../OverlordProject/Materials/Post\PostRGBScanline.h"
#include "../OverlordProject/Materials/Post\PostBlur.h"
#include "Graphics/TextRenderer.h"
#include "Graphics/SpriteFont.h"
#include "Content/ContentManager.h"
#include "Scenegraph/SceneManager.h"
BombermanLevel::BombermanLevel(void) :
	GameScene(L"BombermanLevel")
{
}

BombermanLevel::~BombermanLevel(void)
{
}

void BombermanLevel::Initialize(const GameContext& gameContext)
{
	// ************************** MENU ************************** 
	//m_MenuBackground = new GameObject();
	//m_MenuBackground->AddComponent(new SpriteComponent(L"./Resources/Sprites/BMAN_Menu_Background.png", XMFLOAT2(0, 0), XMFLOAT4(1, 1, 1, 1)));
	//AddChild(m_MenuBackground);
	//m_MenuBackground->GetTransform()->Translate(0, -2000, .91f);
	m_pFont = ContentManager::Load<SpriteFont>(L"./Resources/SpriteFonts/Consolas_32.fnt");
	float width, height;
	width = OverlordGame::GetGameSettings().Window.Width;
	height = OverlordGame::GetGameSettings().Window.Height;

	auto scoreCard = new GameObject();
	auto cardSprite = new SpriteComponent(L"./Resources/Sprites/BMAN_HUD_Card.png", XMFLOAT2(0, 0), XMFLOAT4(1, 1, 1, 0.7f));
	scoreCard->AddComponent(cardSprite);
	AddChild(scoreCard);
	scoreCard->GetTransform()->Translate(20, 20, 0.001f);
	scoreCard->GetTransform()->Scale(0.3f, 0.5f, 1);

	scoreCard = new GameObject();
	cardSprite = new SpriteComponent(L"./Resources/Sprites/BMAN_HUD_Card.png", XMFLOAT2(1, 0), XMFLOAT4(1, 1, 1, 0.7f));
	scoreCard->AddComponent(cardSprite);
	AddChild(scoreCard);
	scoreCard->GetTransform()->Translate(width - 20, 20, .001f);
	scoreCard->GetTransform()->Scale(0.3f, 0.5f, 1);

	auto victoryObj = new GameObject();
	m_VictoryText = new SpriteComponent(L"./Resources/Sprites/BMAN_HUD_Victory.png", XMFLOAT2(0.5f, 0), XMFLOAT4(1, 1, 1, 0));
	victoryObj->AddComponent(m_VictoryText);
	AddChild(victoryObj);
	victoryObj->GetTransform()->Translate(width * 0.5f, 20, 0.0015f);

	auto gameResultScreen = new GameObject();
	m_ResultScreen = new SpriteComponent(L"./Resources/Sprites/BMAN_HUD_Tie.png", XMFLOAT2(0, 0), XMFLOAT4(1, 1, 1, 0));
	gameResultScreen->AddComponent(m_ResultScreen);
	AddChild(gameResultScreen);
	gameResultScreen->GetTransform()->Translate(0, 0, .002f);

	m_RestartButton = new GameObject();
	m_RestartSprite = new SpriteComponent(L"./Resources/Sprites/BMAN_Menu_Restart.png", XMFLOAT2(0.5f, 0.5f), XMFLOAT4(1, 1, 1, 1));
	m_RestartButton->AddComponent(m_RestartSprite);
	AddChild(m_RestartButton);
	m_RestartButton->GetTransform()->Translate(width * 0.5f, height * 0.5f - 125 + m_MenuOffset, .001f);

	m_MainMenuButton = new GameObject();
	m_MainMenuSprite = new SpriteComponent(L"./Resources/Sprites/BMAN_Menu_Menu.png", XMFLOAT2(0.5f, 0.5f), XMFLOAT4(1, 1, 1, 1));
	m_MainMenuButton->AddComponent(m_MainMenuSprite);
	AddChild(m_MainMenuButton);
	m_MainMenuButton->GetTransform()->Translate(width * 0.5f, height * 0.5f + m_MenuOffset, .001f);

	m_ExitButton = new GameObject();
	m_ExitSprite = new SpriteComponent(L"./Resources/Sprites/BMAN_Menu_Exit.png", XMFLOAT2(0.5f, 0.5f), XMFLOAT4(1, 1, 1, 1));
	m_ExitButton->AddComponent(m_ExitSprite);
	AddChild(m_ExitButton);
	m_ExitButton->GetTransform()->Translate(width * 0.5f, height * 0.5f + 125 + m_MenuOffset, .001f);

	// ************************** MATERIALS ************************** 

	auto pSkinnedMat = new SkinnedDiffuseMaterial();
	pSkinnedMat->SetDiffuseTexture(L"Resources/Textures/BMAN_Char_Diffuse.png");
	gameContext.pMaterialManager->AddMaterial(pSkinnedMat, 100);

	pSkinnedMat = new SkinnedDiffuseMaterial();
	pSkinnedMat->SetDiffuseTexture(L"Resources/Textures/BMAN_Char_Diffuse2.png");
	gameContext.pMaterialManager->AddMaterial(pSkinnedMat, 101);

	auto pColorMat = new DiffuseMaterial();
	pColorMat->SetDiffuseTexture(L"Resources/Textures/BMAN_Wall.png");
	gameContext.pMaterialManager->AddMaterial(pColorMat, 0);

	pColorMat = new DiffuseMaterial();
	pColorMat->SetDiffuseTexture(L"Resources/Textures/BMAN_Bomb.png");
	gameContext.pMaterialManager->AddMaterial(pColorMat, 1);

	pColorMat = new DiffuseMaterial();
	pColorMat->SetDiffuseTexture(L"Resources/Textures/BMAN_Metal.png");
	gameContext.pMaterialManager->AddMaterial(pColorMat, 2);


	pColorMat = new DiffuseMaterial();
	pColorMat->SetDiffuseTexture(L"Resources/Textures/BMAN_Box.png");
	gameContext.pMaterialManager->AddMaterial(pColorMat, 3);

	pColorMat = new DiffuseMaterial();
	pColorMat->SetDiffuseTexture(L"Resources/Textures/BMAN_Floor.png");
	gameContext.pMaterialManager->AddMaterial(pColorMat, 4);

	auto pWaterMat = new WaterMaterial();
	pWaterMat->SetDiffuseTexture(L"Resources/Textures/BMAN_Water_Packed.png");
	pWaterMat->SetFoamIntensity(0.3f);
	pWaterMat->SetWaveHeight(24.0f);
	pWaterMat->SetFoamScale(10);
	pWaterMat->SetOffsetColorMultiplier(0.6f);
	pWaterMat->SetOffsetNoiseScale(20);
	pWaterMat->SetWaterColor(XMFLOAT4(0, 0.25f, 0.5f, 1));
	gameContext.pMaterialManager->AddMaterial(pWaterMat, 5);

	auto pFoamMat = new FoamMaterial();
	pFoamMat->SetFoamTexture(L"Resources/Textures/BMAN_Foam_Packed.png");
	pFoamMat->SetNoiseTexture(L"Resources/Textures/BMAN_Water_Packed.png");
	pFoamMat->SetOffsetNoiseScale(20);
	pFoamMat->SetWaveHeight(24.0f);
	pFoamMat->SetWaterColor(XMFLOAT4(1, 1, 1, 1));
	gameContext.pMaterialManager->AddMaterial(pFoamMat, 6);

	pColorMat = new DiffuseMaterial();
	pColorMat->SetDiffuseTexture(L"Resources/Textures/BMAN_Pickup_BombPower.png");
	gameContext.pMaterialManager->AddMaterial(pColorMat, 20);


	pColorMat = new DiffuseMaterial();
	pColorMat->SetDiffuseTexture(L"Resources/Textures/BMAN_Pickup_BombAmount.png");
	gameContext.pMaterialManager->AddMaterial(pColorMat, 21);

	pColorMat = new DiffuseMaterial();
	pColorMat->SetDiffuseTexture(L"Resources/Textures/BMAN_Pickup_Speed.png");
	gameContext.pMaterialManager->AddMaterial(pColorMat, 22);
	// ************************** FIELD | PLAYERS ************************** 
	m_pField = new PlayingField({ 19, 13 }, gameContext);
	m_pPlayer1 = new Player(m_pField, { 1, 1 }, { 0, 1, 2, 3, 4 }, 100, gameContext);
	m_pPlayer2 = new Player(m_pField, { 17, 11 }, { 5, 6, 7, 8, 9 }, 101, gameContext);

	m_pField->AddPlayer1(m_pPlayer1);
	m_pField->AddPlayer2(m_pPlayer2);
	AddChild(m_pField);
	AddChild(m_pPlayer1);
	AddChild(m_pPlayer2);
	m_Camera = new FixedCamera();
	m_Camera->GetTransform()->Rotate(75, 0, 0);
	AddChild(m_Camera);
	m_Camera->GetTransform()->Translate(0, 30.9f, -8.5f);
	SetActiveCamera(m_Camera->GetComponent<CameraComponent>());
	m_Camera->GetComponent<CameraComponent>()->SetFieldOfView(XM_PIDIV4 * 0.5f);

	// ************************** INPUT ************************** 

	gameContext.pInput->AddInputAction(InputAction(0, Down, 'F'));
	gameContext.pInput->AddInputAction(InputAction(1, Down, 'S'));
	gameContext.pInput->AddInputAction(InputAction(2, Down, 'E'));
	gameContext.pInput->AddInputAction(InputAction(3, Down, 'D'));
	gameContext.pInput->AddInputAction(InputAction(4, Pressed, VK_SPACE));

	gameContext.pInput->AddInputAction(InputAction(5, Down, VK_RIGHT));
	gameContext.pInput->AddInputAction(InputAction(6, Down, VK_LEFT));
	gameContext.pInput->AddInputAction(InputAction(7, Down, VK_UP));
	gameContext.pInput->AddInputAction(InputAction(8, Down, VK_DOWN));
	gameContext.pInput->AddInputAction(InputAction(9, Pressed, VK_END));

	gameContext.pInput->AddInputAction(InputAction(10, Pressed, VK_ESCAPE));


	gameContext.pInput->AddInputAction(InputAction(11, Pressed, VK_PRIOR));
	gameContext.pInput->AddInputAction(InputAction(12, Pressed, VK_NEXT));
	gameContext.pInput->AddInputAction(InputAction(13, Pressed, VK_DELETE));

	gameContext.pInput->AddInputAction(InputAction(14, Pressed, 'J'));
	gameContext.pInput->AddInputAction(InputAction(15, Pressed, 'K'));

	gameContext.pMaterialManager->AddMaterial_PP(new PostGrayscale(), 0);
	gameContext.pMaterialManager->AddMaterial_PP(new PostKuwahara(5), 1);
	gameContext.pMaterialManager->AddMaterial_PP(new PostKuwaharaCharcoal(), 2);

	gameContext.pMaterialManager->AddMaterial_PP(new PostBarrel(), 3);
	gameContext.pMaterialManager->AddMaterial_PP(new PostCircles(256), 4);
	gameContext.pMaterialManager->AddMaterial_PP(new PostEdges(), 5);

	gameContext.pMaterialManager->AddMaterial_PP(new PostMonoScanline(), 6);
	gameContext.pMaterialManager->AddMaterial_PP(new PostRGBScanline(), 7);
	gameContext.pMaterialManager->AddMaterial_PP(new PostKuwahara(2), 8);

	gameContext.pMaterialManager->AddMaterial_PP(new PostBlur(), 10);


	m_PPEffects.resize(9);
	for (size_t i = 0; i < m_PPEffects.size(); i++)
	{
		m_PPEffects[i] = false;
	}


}

void BombermanLevel::Update(const GameContext& gameContext)
{


	if (!m_GameEnded)
	{
		if (gameContext.pInput->IsActionTriggered(10))
		{
			m_Paused = !m_Paused;
			if (m_Paused) {
				AddPostProcessingMaterial(10);
				m_pPlayer1->DisableControls(true);
				m_pPlayer2->DisableControls(true);
			}
			else
			{
				RemovePostProcessingMaterial(10);
				m_pPlayer1->DisableControls(false);
				m_pPlayer2->DisableControls(false);
			}
		}

		if (m_RespawnTimer < 0) return;
		m_pField->Update(gameContext);
		float time = gameContext.pGameTime->GetTotal() * 0.5f;
		m_Camera->GetTransform()->Rotate(75 + sin(time) * 0.5f, sin(time * 0.9f) * 0.5f, 0);

		if (!m_pPlayer1->IsAlive())
		{
			if (!m_RoundEnded)
			{
				m_RoundEnded = true;

				if (m_pPlayer2->IsAlive())
				{
					m_pPlayer2->Dance(gameContext);
					++m_P2Score;

					if (m_P2Score >= 3)
					{
						m_VictoryText->SetColor(XMFLOAT4(1, 1, 1, 1));
						m_GameEnded = true;
						m_Paused = false;
						m_MenuOffset = -719;
						AddPostProcessingMaterial(10);

						float width, height;
						width = OverlordGame::GetGameSettings().Window.Width;
						height = OverlordGame::GetGameSettings().Window.Height;

						m_RestartButton->GetTransform()->Translate(width * 0.5f, height - 250, .001f);
						m_MainMenuButton->GetTransform()->Translate(width * 0.5f, height - 125, .001f);
					}
					m_ResultScreen->SetTexture(L"Resources/Sprites/BMAN_HUD_P2Victory.png");
					m_ResultScreen->SetColor(XMFLOAT4(1, 1, 1, 1));
				}
				else
				{
					m_ResultScreen->SetTexture(L"Resources/Sprites/BMAN_HUD_Tie.png");
					m_ResultScreen->SetColor(XMFLOAT4(1, 1, 1, 1));
				}
			}
			m_RespawnTimer -= gameContext.pGameTime->GetElapsed();

			if (m_RespawnTimer < 0)
			{
				Reset(gameContext);
			}
		}
		else if (!m_pPlayer2->IsAlive())
		{
			if (!m_RoundEnded)
			{
				m_RoundEnded = true;
				m_pPlayer1->Dance(gameContext);
				++m_P1Score;

				if (m_P1Score >= 3)
				{
					m_VictoryText->SetColor(XMFLOAT4(1, 1, 1, 1));
					m_GameEnded = true;
					m_Paused = false;
					m_MenuOffset = -719;
					AddPostProcessingMaterial(10);

					float width, height;
					width = OverlordGame::GetGameSettings().Window.Width;
					height = OverlordGame::GetGameSettings().Window.Height;

					m_RestartButton->GetTransform()->Translate(width * 0.5f, height - 250, .001f);
					m_MainMenuButton->GetTransform()->Translate(width * 0.5f, height - 125, .001f);
				}

				m_ResultScreen->SetTexture(L"Resources/Sprites/BMAN_HUD_P1Victory.png");
				m_ResultScreen->SetColor(XMFLOAT4(1, 1, 1, 1));
			}

			m_RespawnTimer -= gameContext.pGameTime->GetElapsed();

			if (m_RespawnTimer < 0)
			{
				Reset(gameContext);
			}
		}
	}
	else
	{

	}

	UpdateUI(gameContext);
}

void BombermanLevel::Reset(const GameContext& gameContext, bool full)
{
	if (full)
	{
		m_P1Score = 0;
		m_P2Score = 0;
		
		if (m_GameEnded)
		{
			RemovePostProcessingMaterial(10);
		}

		m_GameEnded = false;
		m_VictoryText->SetColor(XMFLOAT4(1, 1, 1, 0));
	}
	m_ResultScreen->SetColor(XMFLOAT4(1, 1, 1, 0));

	m_RespawnTimer = 5;
	m_RoundEnded = false;
	RemoveChild(m_pField);
	RemoveChild(m_pPlayer1);
	RemoveChild(m_pPlayer2);

	m_pField = new PlayingField({ 19, 13 }, gameContext);
	m_pPlayer1 = new Player(m_pField, { 1, 1 }, { 0, 1, 2, 3, 4 }, 100, gameContext);
	m_pPlayer2 = new Player(m_pField, { 17, 11 }, { 5, 6, 7, 8, 9 }, 101, gameContext);
	m_pField->AddPlayer1(m_pPlayer1);
	m_pField->AddPlayer2(m_pPlayer2);
	AddChild(m_pField);
	AddChild(m_pPlayer1);
	AddChild(m_pPlayer2);
}

void BombermanLevel::UpdateUI(const GameContext& gameContext)
{
	if (m_GameEnded)
	{
		float width, height;
		width = OverlordGame::GetGameSettings().Window.Width;
		height = OverlordGame::GetGameSettings().Window.Height;
		POINT mousePos = gameContext.pInput->GetMousePosition();

		POINT restartMenuPos;
		restartMenuPos.x = width * 0.5f;
		restartMenuPos.y = height - 250;

		POINT mainMenuPos;
		mainMenuPos.x = width * 0.5f;
		mainMenuPos.y = height - 125;


		if (abs(mousePos.x - restartMenuPos.x) < 180 && abs(mousePos.y - restartMenuPos.y) < 50)
		{
			if (gameContext.pInput->IsMouseButtonDown(1) && !gameContext.pInput->IsMouseButtonDown(1, true))
			{
				Reset(gameContext, true);
				m_Paused = false;
				RemovePostProcessingMaterial(10);
				m_pPlayer1->DisableControls(false);
				m_pPlayer2->DisableControls(false);
			}

			if (!m_RestartSelected) {
				m_RestartSelected = true;
				m_RestartSprite->SetTexture(L"./Resources/Sprites/BMAN_Menu_Restart_Selected.png");
			}
		}
		else
		{
			if (m_RestartSelected)
			{
				m_RestartSelected = false;
				m_RestartSprite->SetTexture(L"./Resources/Sprites/BMAN_Menu_Restart.png");
			}
		}

		if (abs(mousePos.x - mainMenuPos.x) < 180 && abs(mousePos.y - mainMenuPos.y) < 50)
		{
			if (gameContext.pInput->IsMouseButtonDown(1) && !gameContext.pInput->IsMouseButtonDown(1, true))
			{
				Reset(gameContext, true);
				m_MenuOffset = -719;
				m_Paused = false;
				RemovePostProcessingMaterial(10);
				SceneManager::GetInstance()->SetActiveGameScene(L"MainMenu");
			}

			if (!m_MenuSelected) {
				m_MenuSelected = true;
				m_MainMenuSprite->SetTexture(L"./Resources/Sprites/BMAN_Menu_Menu_Selected.png");
			}
		}
		else
		{
			if (m_MenuSelected)
			{
				m_MenuSelected = false;
				m_MainMenuSprite->SetTexture(L"./Resources/Sprites/BMAN_Menu_Menu.png");
			}
		}
	}
	else
	{
		if (m_Paused)
		{
			if (m_MenuOffset < 0)
			{
				float width, height;
				width = OverlordGame::GetGameSettings().Window.Width;
				height = OverlordGame::GetGameSettings().Window.Height;

				float speed = 1000;
				float m = abs(m_MenuOffset) / 100;
				m_MenuOffset += m * speed * gameContext.pGameTime->GetElapsed();
				m_RestartButton->GetTransform()->Translate(width * 0.5f, height * 0.5f - 125 + m_MenuOffset, .001f);
				m_MainMenuButton->GetTransform()->Translate(width * 0.5f, height * 0.5f + m_MenuOffset, .001f);
				m_ExitButton->GetTransform()->Translate(width * 0.5f, height * 0.5f + 125 + m_MenuOffset, .001f);
			}

			float width, height;
			width = OverlordGame::GetGameSettings().Window.Width;
			height = OverlordGame::GetGameSettings().Window.Height;
			POINT mousePos = gameContext.pInput->GetMousePosition();

			POINT restartMenuPos;
			restartMenuPos.x = width * 0.5f;
			restartMenuPos.y = height * 0.5f - 125 + m_MenuOffset;

			POINT mainMenuPos;
			mainMenuPos.x = width * 0.5f;
			mainMenuPos.y = height * 0.5f + m_MenuOffset;

			POINT exitMenuPos;
			exitMenuPos.x = width * 0.5f;
			exitMenuPos.y = height * 0.5f + 125 + m_MenuOffset;

			if (abs(mousePos.x - restartMenuPos.x) < 180 && abs(mousePos.y - restartMenuPos.y) < 50)
			{
				if (gameContext.pInput->IsMouseButtonDown(1) && !gameContext.pInput->IsMouseButtonDown(1, true))
				{
					Reset(gameContext, true);
					m_Paused = false;
					RemovePostProcessingMaterial(10);
					m_pPlayer1->DisableControls(false);
					m_pPlayer2->DisableControls(false);
				}

				if (!m_RestartSelected) {
					m_RestartSelected = true;
					m_RestartSprite->SetTexture(L"./Resources/Sprites/BMAN_Menu_Restart_Selected.png");
				}
			}
			else
			{
				if (m_RestartSelected)
				{
					m_RestartSelected = false;
					m_RestartSprite->SetTexture(L"./Resources/Sprites/BMAN_Menu_Restart.png");
				}
			}

			if (abs(mousePos.x - mainMenuPos.x) < 180 && abs(mousePos.y - mainMenuPos.y) < 50)
			{
				if (gameContext.pInput->IsMouseButtonDown(1) && !gameContext.pInput->IsMouseButtonDown(1, true))
				{
					Reset(gameContext, true);
					m_MenuOffset = -719;
					m_Paused = false;
					RemovePostProcessingMaterial(10);
					SceneManager::GetInstance()->SetActiveGameScene(L"MainMenu");
				}

				if (!m_MenuSelected) {
					m_MenuSelected = true;
					m_MainMenuSprite->SetTexture(L"./Resources/Sprites/BMAN_Menu_Menu_Selected.png");
				}
			}
			else
			{
				if (m_MenuSelected)
				{
					m_MenuSelected = false;
					m_MainMenuSprite->SetTexture(L"./Resources/Sprites/BMAN_Menu_Menu.png");
				}
			}

			if (abs(mousePos.x - exitMenuPos.x) < 180 && abs(mousePos.y - exitMenuPos.y) < 50)
			{
				if (gameContext.pInput->IsMouseButtonDown(1) && !gameContext.pInput->IsMouseButtonDown(1, true))
				{
					OverlordGame::Quit();
				}

				if (!m_ExitSelected) {
					m_ExitSelected = true;
					m_ExitSprite->SetTexture(L"./Resources/Sprites/BMAN_Menu_Exit_Selected.png");
				}
			}
			else
			{
				if (m_ExitSelected)
				{
					m_ExitSelected = false;
					m_ExitSprite->SetTexture(L"./Resources/Sprites/BMAN_Menu_Exit.png");
				}
			}

		}
		else
		{
			if (m_MenuOffset > -720)
			{
				float width, height;
				width = OverlordGame::GetGameSettings().Window.Width;
				height = OverlordGame::GetGameSettings().Window.Height;

				float speed = 1000;
				float m = (720 - abs(m_MenuOffset)) / 100;
				m_MenuOffset -= m * speed * gameContext.pGameTime->GetElapsed();
				m_RestartButton->GetTransform()->Translate(width * 0.5f, height * 0.5f - 125 + m_MenuOffset, .001f);
				m_MainMenuButton->GetTransform()->Translate(width * 0.5f, height * 0.5f + m_MenuOffset, .001f);
				m_ExitButton->GetTransform()->Translate(width * 0.5f, height * 0.5f + 125 + m_MenuOffset, .001f);
			}
		}
	}

	

	if (gameContext.pInput->IsActionTriggered(11)) {
		PreviousPPPreset(gameContext);
	}

	if (gameContext.pInput->IsActionTriggered(12)) {
		NextPPPreset(gameContext);
	}


	if (gameContext.pInput->IsActionTriggered(13)) {
		ClearPPEffects(gameContext);
	}

	if (gameContext.pInput->IsActionTriggered(14)) {
		NextPPEffect(gameContext);
	}

	if (gameContext.pInput->IsActionTriggered(15)) {
		PreviousPPEffect(gameContext);
	}
}

void BombermanLevel::ClearPPEffects(const GameContext& gameContext)
{
	for (size_t i = 0; i < m_PPEffects.size(); i++)
	{
		if (m_PPEffects[i]) {
			RemovePostProcessingMaterial(i);
			m_PPEffects[i] = false;
		}
	}
}

void BombermanLevel::NextPPEffect(const GameContext & gameContext)
{
	ClearPPEffects(gameContext);
	m_PPEffect = (m_PPEffect + 1) % (m_PPEffects.size() + 1);

	if (m_PPEffect != 0) {
		AddPostProcessingMaterial(m_PPEffect - 1);
		m_PPEffects[m_PPEffect - 1] = true;
	}
}

void BombermanLevel::PreviousPPEffect(const GameContext & gameContext)
{
	ClearPPEffects(gameContext);
	--m_PPEffect;
	if (m_PPEffect < 0) m_PPEffect = m_PPEffects.size();

	if (m_PPEffect != 0) {
		AddPostProcessingMaterial(m_PPEffect - 1);
		m_PPEffects[m_PPEffect - 1] = true;
	}
}

void BombermanLevel::NextPPPreset(const GameContext & gameContext)
{
	ClearPPEffects(gameContext);
	++m_Preset;
	if (m_Preset > 5)
	{
		m_Preset = 0;
	}
	else
	{
		switch (m_Preset)
		{
		case 1:
			AddPostProcessingMaterial(1);
			m_PPEffects[1] = true;
			AddPostProcessingMaterial(7);
			m_PPEffects[7] = true;
			break;
		case 2:
			AddPostProcessingMaterial(0);
			m_PPEffects[0] = true;
			AddPostProcessingMaterial(6);
			m_PPEffects[6] = true;
			break;
		case 3:
			AddPostProcessingMaterial(7);
			m_PPEffects[7] = true;
			AddPostProcessingMaterial(5);
			m_PPEffects[5] = true;
			break;
		case 4:
			AddPostProcessingMaterial(6);
			m_PPEffects[6] = true;
			AddPostProcessingMaterial(5);
			m_PPEffects[5] = true;
			break;
		case 5:
			AddPostProcessingMaterial(6);
			m_PPEffects[6] = true;
			AddPostProcessingMaterial(5);
			m_PPEffects[5] = true;
			AddPostProcessingMaterial(8);
			m_PPEffects[8] = true;
			break;
		default:
			break;
		}
	}
	// PRESET: 1 - 7
	// PRESET: 0 - 6
	// PRESET: 7 - 5
	// PRESET: 6 - 5
	// PRESET: 6 - 5 - 8
}

void BombermanLevel::PreviousPPPreset(const GameContext & gameContext)
{
	ClearPPEffects(gameContext);
	--m_Preset;
	if (m_Preset < 0)
	{
		m_Preset = 5;
	}

	switch (m_Preset)
	{
	case 1:
		AddPostProcessingMaterial(1);
		m_PPEffects[1] = true;
		AddPostProcessingMaterial(7);
		m_PPEffects[7] = true;
		break;
	case 2:
		AddPostProcessingMaterial(0);
		m_PPEffects[0] = true;
		AddPostProcessingMaterial(6);
		m_PPEffects[6] = true;
		break;
	case 3:
		AddPostProcessingMaterial(7);
		m_PPEffects[7] = true;
		AddPostProcessingMaterial(5);
		m_PPEffects[5] = true;
		break;
	case 4:
		AddPostProcessingMaterial(6);
		m_PPEffects[6] = true;
		AddPostProcessingMaterial(5);
		m_PPEffects[5] = true;
		break;
	case 5:
		AddPostProcessingMaterial(6);
		m_PPEffects[6] = true;
		AddPostProcessingMaterial(5);
		m_PPEffects[5] = true;
		AddPostProcessingMaterial(8);
		m_PPEffects[8] = true;
		break;
	default:
		break;
	}

}

void BombermanLevel::Draw(const GameContext& gameContext)
{
	wstringstream sP1;
	sP1 << "P1: " << m_P1Score;
	TextRenderer::GetInstance()->DrawText(m_pFont, sP1.str(), {30, 30}, XMFLOAT4(Colors::Black));

	float width, height;
	width = OverlordGame::GetGameSettings().Window.Width;
	height = OverlordGame::GetGameSettings().Window.Height;
	wstringstream sP2;
	sP2 << "P2: " << m_P2Score;
	TextRenderer::GetInstance()->DrawText(m_pFont, sP2.str(), { width - 118, 30 }, XMFLOAT4(Colors::Black));

	wstringstream s;
	s << "FPS: " << gameContext.pGameTime->GetFPS();
	TextRenderer::GetInstance()->DrawText(m_pFont, s.str(), { 20, height - 30}, XMFLOAT4(Colors::Black));

	UNREFERENCED_PARAMETER(gameContext);
}
