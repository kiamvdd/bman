//Precompiled Header [ALWAYS ON TOP IN CPP]
#include <stdafx.h>
#include "PostCircles.h"
#include "Graphics/RenderTarget.h"

PostCircles::PostCircles(float pixelsAcross)
	: PostProcessingMaterial(L"./Resources/Effects/Post/Circles.fx"),
	m_pTextureMapVariabele(nullptr),
	m_PixelsAcross(pixelsAcross)
{
}

PostCircles::~PostCircles(void)
{
}

void PostCircles::LoadEffectVariables()
{
	auto effectVar = m_pEffect->GetVariableByName("gTexture");
	m_pTextureMapVariabele = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;

	effectVar = m_pEffect->GetVariableByName("pixelsAcross");
	m_pPixelAmountVar = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;
	//Bind the 'gTexture' variable with 'm_pTextureMapVariable'
	//Check if valid!
}

void PostCircles::UpdateEffectVariables(RenderTarget* rendertarget)
{
	if (m_pTextureMapVariabele)
		m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());

	if (m_pPixelAmountVar)
		m_pPixelAmountVar->SetInt(m_PixelsAcross);
	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
}