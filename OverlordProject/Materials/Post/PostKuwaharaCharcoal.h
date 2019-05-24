#pragma once
#include "Graphics/PostProcessingMaterial.h"

class ID3D11EffectShaderResourceVariable;

class PostKuwaharaCharcoal : public PostProcessingMaterial
{
public:
	PostKuwaharaCharcoal();
	~PostKuwaharaCharcoal();

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(RenderTarget* rendertarget);

	ID3DX11EffectShaderResourceVariable* m_pTextureMapVariabele;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PostKuwaharaCharcoal(const PostKuwaharaCharcoal &obj);
	PostKuwaharaCharcoal& operator=(const PostKuwaharaCharcoal& obj);
};
