#include "stdafx.h"
#include "WaterMaterial.h"
#include "Content\ContentManager.h"
#include "Graphics\TextureData.h"

ID3DX11EffectShaderResourceVariable* WaterMaterial::m_pWaterSRVVariable = nullptr;

WaterMaterial::WaterMaterial() : Material(L"./Resources/Effects/Water.fx", L"")
{
}

WaterMaterial::~WaterMaterial()
{
}

void WaterMaterial::SetDiffuseTexture(const wstring & assetFile)
{
	m_pWaterTexture = ContentManager::Load<TextureData>(assetFile);
}

void WaterMaterial::SetFoamScale(float scale)
{
	m_FoamScale = scale;
}

void WaterMaterial::SetFoamIntensity(float intensity)
{
	m_FoamIntensity = intensity;
}

void WaterMaterial::SetWaterColor(XMFLOAT4 color)
{
	m_WaterColor = color;
}

void WaterMaterial::SetWaveHeight(float height)
{
	m_WaveHeight = height;
}

void WaterMaterial::SetOffsetNoiseScale(float scale)
{
	m_NoiseScale = scale;
}

void WaterMaterial::SetOffsetColorMultiplier(float mult)
{
	m_ColorMultiplier = mult;
}

void WaterMaterial::LoadEffectVariables()
{
	auto effectVar = m_pEffect->GetVariableByName("gTexturePacked");
	m_pWaterSRVVariable = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;

	m_pFoamScale = m_pEffect->GetVariableByName("gFoamScale")->AsScalar();
	m_pFoamIntensity = m_pEffect->GetVariableByName("gFoamIntensity")->AsScalar();
	m_pColorMultiplier = m_pEffect->GetVariableByName("gOffsetColorMultiplier")->AsScalar();
	m_pNoiseScale = m_pEffect->GetVariableByName("gOffsetNoiseScale")->AsScalar();
	m_pTime = m_pEffect->GetVariableByName("gTime")->AsScalar();
	m_pWaterColor = m_pEffect->GetVariableByName("gWaterColor")->AsVector();
	m_pWaveHeight = m_pEffect->GetVariableByName("gWaveHeight")->AsScalar();
}

void WaterMaterial::UpdateEffectVariables(const GameContext & gameContext, ModelComponent * pModelComponent)
{
	if (m_pWaterSRVVariable)
		m_pWaterSRVVariable->SetResource(m_pWaterTexture->GetShaderResourceView());

	if (m_pFoamScale) {
		m_pFoamScale->SetFloat(m_FoamScale);
	}

	if (m_pWaveHeight) {
		m_pWaveHeight->SetFloat(m_WaveHeight);
	}

	if (m_pFoamIntensity) {
		m_pFoamIntensity->SetFloat(m_FoamIntensity);
	}

	if (m_pNoiseScale) {
		m_pNoiseScale->SetFloat(m_NoiseScale);
	}

	if (m_pColorMultiplier) {
		m_pColorMultiplier->SetFloat(m_ColorMultiplier);
	}

	if (m_pTime) {
		m_pTime->SetFloat(gameContext.pGameTime->GetTotal());
	}

	if (m_pWaterColor) {
		m_pWaterColor->SetFloatVector(reinterpret_cast<float*>(&m_WaterColor));
	}

	UNREFERENCED_PARAMETER(pModelComponent);
}
