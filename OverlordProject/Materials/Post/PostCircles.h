#pragma once
#include "Graphics/PostProcessingMaterial.h"

class ID3D11EffectShaderResourceVariable;

class PostCircles : public PostProcessingMaterial
{
public:
	PostCircles(float pixelsAcross);
	~PostCircles();

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(RenderTarget* rendertarget);

	ID3DX11EffectShaderResourceVariable* m_pTextureMapVariabele;
	ID3DX11EffectScalarVariable* m_pPixelAmountVar;

	int m_PixelsAcross;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PostCircles(const PostCircles &obj);
	PostCircles& operator=(const PostCircles& obj);
};
