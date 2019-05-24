#pragma once
#include "Graphics/Material.h"
class TextureData;

class DiffuseMaterial : public Material 
{
public:
	DiffuseMaterial();
	~DiffuseMaterial();

	void SetDiffuseTexture(const wstring& assetFile);

private:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:
	TextureData* m_pDiffuseTexture;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVVariable;

	DiffuseMaterial(const DiffuseMaterial &obj) = delete;
	DiffuseMaterial &operator=(const DiffuseMaterial &obj) = delete;
};
