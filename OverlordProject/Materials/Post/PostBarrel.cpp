//Precompiled Header [ALWAYS ON TOP IN CPP]
#include <stdafx.h>
#include "PostBarrel.h"
#include "Graphics/RenderTarget.h"

PostBarrel::PostBarrel()
	: PostProcessingMaterial(L"./Resources/Effects/Post/BarrelDistort.fx"),
	m_pTextureMapVariabele(nullptr)
{
}

PostBarrel::~PostBarrel(void)
{
}

void PostBarrel::LoadEffectVariables()
{
	auto effectVar = m_pEffect->GetVariableByName("gTexture");
	m_pTextureMapVariabele = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;
	//Bind the 'gTexture' variable with 'm_pTextureMapVariable'
	//Check if valid!
}

void PostBarrel::UpdateEffectVariables(RenderTarget* rendertarget)
{
	if (m_pTextureMapVariabele)
		m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());
	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
}