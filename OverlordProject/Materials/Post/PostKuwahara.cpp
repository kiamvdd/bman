//Precompiled Header [ALWAYS ON TOP IN CPP]
#include <stdafx.h>
#include "PostKuwahara.h"
#include "Graphics/RenderTarget.h"

PostKuwahara::PostKuwahara(int regionsize)
	: PostProcessingMaterial(L"./Resources/Effects/Post/KuwaharaPainted.fx"),
	m_pTextureMapVariabele(nullptr),
	m_RegionSize(regionsize)
{
}

PostKuwahara::~PostKuwahara(void)
{
}

void PostKuwahara::LoadEffectVariables()
{
	auto effectVar = m_pEffect->GetVariableByName("gTexture");
	m_pTextureMapVariabele = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;

	effectVar = m_pEffect->GetVariableByName("regionsize");
	m_pRegionSizeVar = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;
	//Bind the 'gTexture' variable with 'm_pTextureMapVariable'
	//Check if valid!
}

void PostKuwahara::UpdateEffectVariables(RenderTarget* rendertarget)
{
	if (m_pTextureMapVariabele)
		m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());

	if (m_pRegionSizeVar)
		m_pRegionSizeVar->SetInt(m_RegionSize);
	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
}