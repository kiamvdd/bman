//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "GameScene.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "../Diagnostics/Logger.h"
#include "../Prefabs/Prefabs.h"
#include "../Components/Components.h"
#include "../Diagnostics/DebugRenderer.h"
#include "../Physx/PhysxProxy.h"
#include <SoundManager.h>
#include "../Graphics/SpriteRenderer.h"
#include "../Graphics/TextRenderer.h"
#include "../Graphics/PostProcessingMaterial.h"
#include "../Base/OverlordGame.h"
#include "../Graphics/RenderTarget.h"


GameScene::GameScene(wstring sceneName):
	m_pChildren(vector<GameObject*>()),
	m_GameContext(GameContext()),
	m_IsInitialized(false),
	m_SceneName(sceneName),
	m_pDefaultCamera(nullptr),
	m_pActiveCamera(nullptr),
	m_pPhysxProxy(nullptr),
	m_PostProcessingMaterialIds(vector<UINT>())
{
}


GameScene::~GameScene(void)
{
	SafeDelete(m_GameContext.pGameTime);
	SafeDelete(m_GameContext.pInput);
	SafeDelete(m_GameContext.pMaterialManager);

	for(GameObject* pChild:m_pChildren)
	{
		SafeDelete(pChild);
	}

	SafeDelete(m_pPhysxProxy);
}

void GameScene::AddChild(GameObject* obj)
{
#if _DEBUG
	if(obj->m_pParentScene)
	{
		if(obj->m_pParentScene == this)
			Logger::LogWarning(L"GameScene::AddChild > GameObject is already attached to this GameScene");
		else
			Logger::LogWarning(L"GameScene::AddChild > GameObject is already attached to another GameScene. Detach it from it's current scene before attaching it to another one.");

		return;
	}

	if(obj->m_pParentObject)
	{
		Logger::LogWarning(L"GameScene::AddChild > GameObject is currently attached to a GameObject. Detach it from it's current parent before attaching it to another one.");
		return;
	}
#endif

	obj->m_pParentScene = this;
	obj->RootInitialize(m_GameContext);
	m_pChildren.push_back(obj);
}

void GameScene::RemoveChild(GameObject* obj, bool deleteObject)
{
	auto it = find(m_pChildren.begin(), m_pChildren.end(), obj);

#if _DEBUG
	if(it == m_pChildren.end())
	{
		Logger::LogWarning(L"GameScene::RemoveChild > GameObject to remove is not attached to this GameScene!");
		return;
	}
#endif

	m_pChildren.erase(it);
	if(deleteObject)
	{
		delete obj;
		obj = nullptr;
	}
	else
		obj->m_pParentScene = nullptr;
}

void GameScene::RootInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	if(m_IsInitialized)
		return;

	//Create DefaultCamera
	auto freeCam = new FreeCamera();
	freeCam->SetRotation(30, 0);
	freeCam->GetTransform()->Translate(0,50,-80);
	AddChild(freeCam);
	m_pDefaultCamera = freeCam->GetComponent<CameraComponent>();
	m_pActiveCamera = m_pDefaultCamera;
	m_GameContext.pCamera = m_pDefaultCamera;

	//Create GameContext
	m_GameContext.pGameTime = new GameTime();
	m_GameContext.pGameTime->Reset();
	m_GameContext.pGameTime->Stop();

	m_GameContext.pInput = new InputManager();
	m_GameContext.pInput->Initialize();

	m_GameContext.pMaterialManager = new MaterialManager();

	m_GameContext.pDevice = pDevice;
	m_GameContext.pDeviceContext = pDeviceContext;

	// Initialize Physx
	m_pPhysxProxy = new PhysxProxy();
	m_pPhysxProxy->Initialize(this);

	//User-Scene Initialize
	Initialize(m_GameContext);

	//Root-Scene Initialize
	for(GameObject* pChild:m_pChildren)
	{
		pChild->RootInitialize(m_GameContext);
	}

	//Initialize Materials
	m_GameContext.pMaterialManager->Initialize(m_GameContext);

	m_IsInitialized = true;
}

void GameScene::RootUpdate()
{
	m_GameContext.pGameTime->Update();
	m_GameContext.pInput->Update();
	m_GameContext.pCamera = m_pActiveCamera;

	SoundManager::GetInstance()->GetSystem()->update();

	//User-Scene Update
	Update(m_GameContext);

	//Root-Scene Update
	for(GameObject* pChild:m_pChildren)
	{
		pChild->RootUpdate(m_GameContext);
	}

	m_pPhysxProxy->Update(m_GameContext);
}

void GameScene::RootDraw()
{
	//User-Scene Draw
	Draw(m_GameContext);

	//Object-Scene Draw
	for(GameObject* pChild:m_pChildren)
	{
		pChild->RootDraw(m_GameContext);
	}

	//Object-Scene Post-Draw
	for (GameObject* pChild : m_pChildren)
	{
		pChild->RootPostDraw(m_GameContext);
	}

	//Draw PhysX
	m_pPhysxProxy->Draw(m_GameContext);

	//Draw Debug Stuff
	DebugRenderer::Draw(m_GameContext);

	//Post Processing Implementation START
	//************************************
	if (m_PostProcessingMaterialIds.size() > 0) {
		auto pGame = SceneManager::GetInstance()->GetGame();
		RenderTarget* INIT_RT = pGame->GetRenderTarget();
		RenderTarget* PREV_RT = INIT_RT;

		for (size_t i = 0; i < m_PostProcessingMaterialIds.size(); i++) {
			PostProcessingMaterial* CURR_MAT = m_GameContext.pMaterialManager->GetMaterial_PP(m_PostProcessingMaterialIds[i]);
			RenderTarget* TEMP_RT = CURR_MAT->GetRenderTarget();
			pGame->SetRenderTarget(TEMP_RT);
			CURR_MAT->Draw(m_GameContext, PREV_RT);
			PREV_RT = TEMP_RT;
		}
		pGame->SetRenderTarget(INIT_RT);
		SpriteRenderer::GetInstance()->DrawImmediate(m_GameContext, PREV_RT->GetShaderResourceView(), {0, 0});
	}

	SpriteRenderer::GetInstance()->Draw(m_GameContext);
	TextRenderer::GetInstance()->Draw(m_GameContext);
}

void GameScene::RootSceneActivated()
{
	//Start Timer
	m_GameContext.pGameTime->Start();
	SceneActivated();
}

void GameScene::RootSceneDeactivated()
{
	//Stop Timer
	m_GameContext.pGameTime->Stop();
	SceneDeactivated();
}

void GameScene::RootWindowStateChanged(int state, bool active)
{
	//TIMER
	if (state == 0)
	{
		if (active)m_GameContext.pGameTime->Start();
		else m_GameContext.pGameTime->Stop();
	}
}

void GameScene::AddPostProcessingMaterial(UINT materialId_PP)
{
	auto it = find(m_PostProcessingMaterialIds.begin(), m_PostProcessingMaterialIds.end(), materialId_PP);

	if (it == m_PostProcessingMaterialIds.end())
		m_PostProcessingMaterialIds.push_back(materialId_PP);

}

void GameScene::RemovePostProcessingMaterial(UINT materialId_PP)
{
	auto it = find(m_PostProcessingMaterialIds.begin(), m_PostProcessingMaterialIds.end(), materialId_PP);

	if (it != m_PostProcessingMaterialIds.end())
		m_PostProcessingMaterialIds.erase(it);
}

void GameScene::SetActiveCamera(CameraComponent* pCameraComponent)
{ 
	//nullptr for DefaultCamera

	if(m_pActiveCamera!=nullptr)
		m_pActiveCamera->m_IsActive = false;

	m_pActiveCamera = (pCameraComponent)?pCameraComponent:m_pDefaultCamera;
	m_pActiveCamera->m_IsActive = true;
}
