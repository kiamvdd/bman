#pragma once
#include "Graphics/PostProcessingMaterial.h"

class ID3D11EffectShaderResourceVariable;

class PostRGBScanline : public PostProcessingMaterial
{
public:
	PostRGBScanline();
	~PostRGBScanline();

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(RenderTarget* rendertarget);

	ID3DX11EffectShaderResourceVariable* m_pTextureMapVariabele;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PostRGBScanline(const PostRGBScanline &obj);
	PostRGBScanline& operator=(const PostRGBScanline& obj);
};
