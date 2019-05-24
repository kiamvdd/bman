#pragma once
#include "Graphics/PostProcessingMaterial.h"

class ID3D11EffectShaderResourceVariable;

class PostMonoScanline : public PostProcessingMaterial
{
public:
	PostMonoScanline();
	~PostMonoScanline();

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(RenderTarget* rendertarget);

	ID3DX11EffectShaderResourceVariable* m_pTextureMapVariabele;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PostMonoScanline(const PostMonoScanline &obj);
	PostMonoScanline& operator=(const PostMonoScanline& obj);
};
