#include "stdafx.h"
#include "SkyBoxPrefab.h"
#include "../Components/ModelComponent.h"
#include "../Materials/SkyBoxMaterial.h"
SkyBoxPrefab::SkyBoxPrefab(const wstring & assetFile) : m_CubeMapFile(assetFile)
{
}

SkyBoxPrefab::~SkyBoxPrefab()
{
}

void SkyBoxPrefab::Initialize(const GameContext& gameContext) 
{
	auto pBox = new ModelComponent(L"Resources/Meshes/Box.ovm");
	auto pSkyBox = new SkyBoxMaterial();
	pSkyBox->SetSkyBoxTexture(m_CubeMapFile);
	gameContext.pMaterialManager->AddMaterial(pSkyBox, 201);
	pBox->SetMaterial(201);
	AddComponent(pBox);
}