#pragma once
#include "Graphics/Material.h"
class TextureData;

class WaterMaterial : public Material
{
public:
	WaterMaterial();
	~WaterMaterial();

	void SetDiffuseTexture(const wstring& assetFile);
	void SetFoamScale(float scale);
	void SetFoamIntensity(float intensity);
	void SetWaterColor(XMFLOAT4 color);
	void SetWaveHeight(float height);
	void SetOffsetNoiseScale(float scale);
	void SetOffsetColorMultiplier(float mult);
private:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:
	XMFLOAT4 m_WaterColor = XMFLOAT4(0, 0.53f, 1, 1);
	float m_FoamScale = 10;
	float m_FoamIntensity = 0.3f;
	float m_NoiseScale = 20;
	float m_ColorMultiplier = 0.6f;
	float m_WaveHeight = 1.0f;

	TextureData* m_pWaterTexture;
	static ID3DX11EffectShaderResourceVariable* m_pWaterSRVVariable;

	ID3DX11EffectVectorVariable* m_pWaterColor;
	ID3DX11EffectScalarVariable* m_pFoamScale;
	ID3DX11EffectScalarVariable* m_pFoamIntensity;
	ID3DX11EffectScalarVariable* m_pNoiseScale;
	ID3DX11EffectScalarVariable* m_pColorMultiplier;
	ID3DX11EffectScalarVariable* m_pWaveHeight;
	ID3DX11EffectScalarVariable* m_pTime;

	WaterMaterial(const WaterMaterial &obj) = delete;
	WaterMaterial &operator=(const WaterMaterial &obj) = delete;
};
