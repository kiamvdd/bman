#include "stdafx.h"
#include "DiffuseMaterial.h"
#include "Content\ContentManager.h"
#include "Graphics\TextureData.h"

ID3DX11EffectShaderResourceVariable* DiffuseMaterial::m_pDiffuseSRVVariable = nullptr;
DiffuseMaterial::DiffuseMaterial() : Material(L"./Resources/Effects/PosNormTex3D.fx", L"")
{
}

DiffuseMaterial::~DiffuseMaterial()
{
}

void DiffuseMaterial::SetDiffuseTexture(const wstring & assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void DiffuseMaterial::LoadEffectVariables()
{
	auto effectVar = m_pEffect->GetVariableByName("gDiffuseMap");
	m_pDiffuseSRVVariable = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;
}

void DiffuseMaterial::UpdateEffectVariables(const GameContext & gameContext, ModelComponent * pModelComponent)
{
	if (m_pDiffuseSRVVariable)
		m_pDiffuseSRVVariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);
}
