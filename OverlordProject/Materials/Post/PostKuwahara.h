#pragma once
#include "Graphics/PostProcessingMaterial.h"

class ID3D11EffectShaderResourceVariable;

class PostKuwahara : public PostProcessingMaterial
{
public:
	PostKuwahara(int regionsize);
	~PostKuwahara();

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(RenderTarget* rendertarget);

	ID3DX11EffectShaderResourceVariable* m_pTextureMapVariabele;
	ID3DX11EffectScalarVariable* m_pRegionSizeVar;

	int m_RegionSize;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PostKuwahara(const PostKuwahara &obj);
	PostKuwahara& operator=(const PostKuwahara& obj);
};
