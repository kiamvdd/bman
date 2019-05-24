//Precompiled Header [ALWAYS ON TOP IN CPP]
#include <stdafx.h>
#include "PostGrayscale.h"
#include "Graphics/RenderTarget.h"

PostGrayscale::PostGrayscale()
	: PostProcessingMaterial(L"./Resources/Effects/Post/Grayscale.fx"),
	m_pTextureMapVariabele(nullptr)
{
}

PostGrayscale::~PostGrayscale(void)
{
}

void PostGrayscale::LoadEffectVariables()
{
	auto effectVar = m_pEffect->GetVariableByName("gTexture");
	m_pTextureMapVariabele = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;
	//Bind the 'gTexture' variable with 'm_pTextureMapVariable'
	//Check if valid!
}

void PostGrayscale::UpdateEffectVariables(RenderTarget* rendertarget)
{
	if (m_pTextureMapVariabele)
		m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());
	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
}