//Precompiled Header [ALWAYS ON TOP IN CPP]
#include <stdafx.h>
#include "PostKuwaharaCharcoal.h"
#include "Graphics/RenderTarget.h"

PostKuwaharaCharcoal::PostKuwaharaCharcoal()
	: PostProcessingMaterial(L"./Resources/Effects/Post/KuwaharaCharcoal.fx"),
	m_pTextureMapVariabele(nullptr)
{
}

PostKuwaharaCharcoal::~PostKuwaharaCharcoal(void)
{
}

void PostKuwaharaCharcoal::LoadEffectVariables()
{
	auto effectVar = m_pEffect->GetVariableByName("gTexture");
	m_pTextureMapVariabele = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;
	//Bind the 'gTexture' variable with 'm_pTextureMapVariable'
	//Check if valid!
}

void PostKuwaharaCharcoal::UpdateEffectVariables(RenderTarget* rendertarget)
{
	if (m_pTextureMapVariabele)
		m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());
	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
}