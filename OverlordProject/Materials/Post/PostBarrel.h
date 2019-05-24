#pragma once
#include "Graphics/PostProcessingMaterial.h"

class ID3D11EffectShaderResourceVariable;

class PostBarrel : public PostProcessingMaterial
{
public:
	PostBarrel();
	~PostBarrel();

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(RenderTarget* rendertarget);

	ID3DX11EffectShaderResourceVariable* m_pTextureMapVariabele;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PostBarrel(const PostBarrel &obj);
	PostBarrel& operator=(const PostBarrel& obj);
};
