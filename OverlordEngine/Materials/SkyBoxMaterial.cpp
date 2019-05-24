#include "stdafx.h"
#include "SkyBoxMaterial.h"
#include "../Graphics/TextureData.h"
#include "../Content/ContentManager.h"

ID3DX11EffectShaderResourceVariable* SkyBoxMaterial::m_pCubeSRVVariable = nullptr;

SkyBoxMaterial::SkyBoxMaterial() : Material(L"./Resources/Effects/SkyBox.fx")
{
}

SkyBoxMaterial::~SkyBoxMaterial()
{
}

void SkyBoxMaterial::SetSkyBoxTexture(const wstring &assetFile)
{
	m_pCubeTexture = ContentManager::Load<TextureData>(assetFile);
}

void SkyBoxMaterial::LoadEffectVariables()
{
	auto effectVar = m_pEffect->GetVariableByName("m_CubeMap");
	m_pCubeSRVVariable = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;
}

void SkyBoxMaterial::UpdateEffectVariables(const GameContext & gameContext, ModelComponent * pModelComponent)
{
	if (m_pCubeSRVVariable && m_pCubeTexture)
		m_pCubeSRVVariable->SetResource(m_pCubeTexture->GetShaderResourceView());
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);
}
