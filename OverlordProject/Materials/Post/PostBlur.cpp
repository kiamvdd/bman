//Precompiled Header [ALWAYS ON TOP IN CPP]
#include <stdafx.h>
#include "PostBlur.h"
#include "Graphics/RenderTarget.h"

PostBlur::PostBlur()
	: PostProcessingMaterial(L"./Resources/Effects/Post/Blur.fx"),
	m_pTextureMapVariabele(nullptr)
{
}

PostBlur::~PostBlur(void)
{
}

void PostBlur::LoadEffectVariables()
{
	auto effectVar = m_pEffect->GetVariableByName("gTexture");
	m_pTextureMapVariabele = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;
	//Bind the 'gTexture' variable with 'm_pTextureMapVariable'
	//Check if valid!
}

void PostBlur::UpdateEffectVariables(RenderTarget* rendertarget)
{
	if (m_pTextureMapVariabele)
		m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());
	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
}