#include "stdafx.h"
#include "FoamMaterial.h"
#include "Content\ContentManager.h"
#include "Graphics\TextureData.h"

ID3DX11EffectShaderResourceVariable* FoamMaterial::m_pNoiseSRV = nullptr;
ID3DX11EffectShaderResourceVariable* FoamMaterial::m_pFoamSRV = nullptr;

FoamMaterial::FoamMaterial() : Material(L"./Resources/Effects/Foam.fx", L"")
{
}

FoamMaterial::~FoamMaterial()
{
}

void FoamMaterial::SetNoiseTexture(const wstring & assetFile)
{
	m_pNoiseTexture = ContentManager::Load<TextureData>(assetFile);
}

void FoamMaterial::SetFoamTexture(const wstring& assetFile)
{
	m_pFoamTexture = ContentManager::Load<TextureData>(assetFile);
}

void FoamMaterial::SetWaterColor(XMFLOAT4 color)
{
	m_FoamColor = color;
}

void FoamMaterial::SetWaveHeight(float height)
{
	m_WaveHeight = height;
}

void FoamMaterial::SetOffsetNoiseScale(float scale)
{
	m_NoiseScale = scale;
}

void FoamMaterial::LoadEffectVariables()
{
	auto effectVar = m_pEffect->GetVariableByName("gTexturePacked");
	m_pNoiseSRV = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;

	effectVar = m_pEffect->GetVariableByName("gFoamPacked");
	m_pFoamSRV = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;

	m_pNoiseScale = m_pEffect->GetVariableByName("gOffsetNoiseScale")->AsScalar();
	m_pTime = m_pEffect->GetVariableByName("gTime")->AsScalar();
	m_pFoamColor = m_pEffect->GetVariableByName("gFoamColor")->AsVector();
	m_pWaveHeight = m_pEffect->GetVariableByName("gWaveHeight")->AsScalar();
}

void FoamMaterial::UpdateEffectVariables(const GameContext & gameContext, ModelComponent * pModelComponent)
{
	if (m_pNoiseSRV)
		m_pNoiseSRV->SetResource(m_pNoiseTexture->GetShaderResourceView());

	if (m_pFoamSRV)
		m_pFoamSRV->SetResource(m_pFoamTexture->GetShaderResourceView());

	if (m_pWaveHeight) {
		m_pWaveHeight->SetFloat(m_WaveHeight);
	}

	if (m_pNoiseScale) {
		m_pNoiseScale->SetFloat(m_NoiseScale);
	}

	if (m_pTime) {
		m_pTime->SetFloat(gameContext.pGameTime->GetTotal());
	}

	if (m_pFoamColor) {
		m_pFoamColor->SetFloatVector(reinterpret_cast<float*>(&m_FoamColor));
	}

	UNREFERENCED_PARAMETER(pModelComponent);
}
