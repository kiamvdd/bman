#pragma once
#include "Graphics/Material.h"
class TextureData;

class FoamMaterial : public Material
{
public:
	FoamMaterial();
	~FoamMaterial();

	void SetNoiseTexture(const wstring& assetFile);
	void SetFoamTexture(const wstring& assetFile);
	void SetWaterColor(XMFLOAT4 color);
	void SetWaveHeight(float height);
	void SetOffsetNoiseScale(float scale);
private:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:
	XMFLOAT4 m_FoamColor = XMFLOAT4(0, 0.53f, 1, 1);
	float m_NoiseScale = 20;
	float m_WaveHeight = 1.0f;

	TextureData* m_pNoiseTexture;
	static ID3DX11EffectShaderResourceVariable* m_pNoiseSRV;
	TextureData* m_pFoamTexture;
	static ID3DX11EffectShaderResourceVariable* m_pFoamSRV;

	ID3DX11EffectVectorVariable* m_pFoamColor;
	ID3DX11EffectScalarVariable* m_pNoiseScale;
	ID3DX11EffectScalarVariable* m_pWaveHeight;
	ID3DX11EffectScalarVariable* m_pTime;

	FoamMaterial(const FoamMaterial &obj) = delete;
	FoamMaterial &operator=(const FoamMaterial &obj) = delete;
};
