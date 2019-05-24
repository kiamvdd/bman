//Precompiled Header [ALWAYS ON TOP IN CPP]
#include <stdafx.h>
#include "PostEdges.h"
#include "Graphics/RenderTarget.h"

PostEdges::PostEdges()
	: PostProcessingMaterial(L"./Resources/Effects/Post/Edges.fx"),
	m_pTextureMapVariabele(nullptr)
{
}

PostEdges::~PostEdges(void)
{
}

void PostEdges::LoadEffectVariables()
{
	auto effectVar = m_pEffect->GetVariableByName("gTexture");
	m_pTextureMapVariabele = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;
	//Bind the 'gTexture' variable with 'm_pTextureMapVariable'
	//Check if valid!
}

void PostEdges::UpdateEffectVariables(RenderTarget* rendertarget)
{
	if (m_pTextureMapVariabele)
		m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());
	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
}