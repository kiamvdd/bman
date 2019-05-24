//Precompiled Header [ALWAYS ON TOP IN CPP]
#include <stdafx.h>
#include "PostRGBScanline.h"
#include "Graphics/RenderTarget.h"

PostRGBScanline::PostRGBScanline()
	: PostProcessingMaterial(L"./Resources/Effects/Post/RGBScanline.fx"),
	m_pTextureMapVariabele(nullptr)
{
}

PostRGBScanline::~PostRGBScanline(void)
{
}

void PostRGBScanline::LoadEffectVariables()
{
	auto effectVar = m_pEffect->GetVariableByName("gTexture");
	m_pTextureMapVariabele = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;
	//Bind the 'gTexture' variable with 'm_pTextureMapVariable'
	//Check if valid!
}

void PostRGBScanline::UpdateEffectVariables(RenderTarget* rendertarget)
{
	if (m_pTextureMapVariabele)
		m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());
	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
}