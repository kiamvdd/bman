#pragma once
#include "Graphics/PostProcessingMaterial.h"

class ID3D11EffectShaderResourceVariable;

class PostEdges : public PostProcessingMaterial
{
public:
	PostEdges();
	~PostEdges();

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(RenderTarget* rendertarget);

	ID3DX11EffectShaderResourceVariable* m_pTextureMapVariabele;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PostEdges(const PostEdges &obj);
	PostEdges& operator=(const PostEdges& obj);
};
