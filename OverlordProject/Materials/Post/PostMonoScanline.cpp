//Precompiled Header [ALWAYS ON TOP IN CPP]
#include <stdafx.h>
#include "PostMonoScanline.h"
#include "Graphics/RenderTarget.h"

PostMonoScanline::PostMonoScanline()
	: PostProcessingMaterial(L"./Resources/Effects/Post/MonoScanline.fx"),
	m_pTextureMapVariabele(nullptr)
{
}

PostMonoScanline::~PostMonoScanline(void)
{
}

void PostMonoScanline::LoadEffectVariables()
{
	auto effectVar = m_pEffect->GetVariableByName("gTexture");
	m_pTextureMapVariabele = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;
	//Bind the 'gTexture' variable with 'm_pTextureMapVariable'
	//Check if valid!
}

void PostMonoScanline::UpdateEffectVariables(RenderTarget* rendertarget)
{
	if (m_pTextureMapVariabele)
		m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());
	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
}