#pragma once
#include "../Graphics/Material.h"
class TextureData;
class SkyBoxMaterial : public Material 
{
public:
	SkyBoxMaterial();
	~SkyBoxMaterial();
public:
	void SetSkyBoxTexture(const wstring &assetFile);
protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);
private:
	TextureData* m_pCubeTexture;
	static ID3DX11EffectShaderResourceVariable* m_pCubeSRVVariable;
private:
	SkyBoxMaterial(const SkyBoxMaterial &obj) = delete;
	SkyBoxMaterial& operator=(const SkyBoxMaterial &obj) = delete;
};