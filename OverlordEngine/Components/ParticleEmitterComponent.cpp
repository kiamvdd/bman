//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ParticleEmitterComponent.h"
#include "../Helpers/EffectHelper.h"
#include "../Content/ContentManager.h"
#include "../Content/TextureDataLoader.h"
#include "../Graphics/TextureData.h"
#include "../Graphics/Particle.h"
#include "../Components/TransformComponent.h"
#include "../Diagnostics/Logger.h"


ParticleEmitterComponent::ParticleEmitterComponent(const wstring& assetFile, int particleCount) :
	m_ParticleCount(particleCount),
	m_AssetFile(assetFile)
{
	m_Particles.resize(particleCount);
	for (int i = 0; i < m_Particles.size(); i++) {
		m_Particles[i] = new Particle(m_Settings);
	}
}


ParticleEmitterComponent::~ParticleEmitterComponent(void)
{
	for (size_t i = 0; i < m_Particles.size(); i++) {
		delete m_Particles[i];
		m_Particles[i] = nullptr;
	}

	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);
}

void ParticleEmitterComponent::Initialize(const GameContext& gameContext)
{
	LoadEffect(gameContext);
	CreateVertexBuffer(gameContext);
	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::LoadEffect(const GameContext& gameContext)
{
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"./Resources/Effects/ParticleRenderer.fx");

	m_pDefaultTechnique = m_pEffect->GetTechniqueByIndex(0);

	auto effectVar = m_pEffect->GetVariableByName("gViewInverse");
	m_pViewInverseVariable = (effectVar->IsValid()) ? effectVar->AsMatrix() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gWorldViewProj");
	m_pWvpVariable = (effectVar->IsValid()) ? effectVar->AsMatrix() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gParticleTexture");
	m_pTextureVariable = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;

	EffectHelper::BuildInputLayout(gameContext.pDevice, m_pDefaultTechnique, &m_pInputLayout, m_pInputLayoutSize);

	//Build InputLayout
}

void ParticleEmitterComponent::CreateVertexBuffer(const GameContext& gameContext)
{
	if (m_pVertexBuffer) {
		SafeRelease(m_pVertexBuffer);
	}

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = m_ParticleCount * sizeof(ParticleVertex);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D10_CPU_ACCESS_FLAG::D3D10_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	gameContext.pDevice->CreateBuffer(&bd, NULL, &m_pVertexBuffer);
}

void ParticleEmitterComponent::Update(const GameContext& gameContext)
{
	auto avgEnergy = m_Settings.MinEnergy + (m_Settings.MaxEnergy - m_Settings.MinEnergy / 2.f);
	auto particleInterval = avgEnergy / m_ParticleCount;

	m_LastParticleInit += gameContext.pGameTime->GetElapsed();

	m_ActiveParticles = 0;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	auto hr = gameContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	Logger::LogHResult(hr, L"ParticleEmitterComponent::Update > Map");

	ParticleVertex* pBuffer = (ParticleVertex*)mappedResource.pData;

	for each(auto particle in m_Particles)
	{
		particle->Update(gameContext);

		if (particle->IsActive()) {
			auto vertInfo = particle->GetVertexInfo();
			auto bufData = (pBuffer + m_ActiveParticles);
			(*bufData) = vertInfo;
			++m_ActiveParticles;
		} else if (m_LastParticleInit >= particleInterval && m_Enabled) {
			particle->Init(GetTransform()->GetWorldPosition());

			auto vertInfo = particle->GetVertexInfo();
			auto bufData = (pBuffer + m_ActiveParticles);
			(*bufData) = vertInfo;
			++m_ActiveParticles;

			m_LastParticleInit = 0;
		}
	}

	gameContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

void ParticleEmitterComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void ParticleEmitterComponent::PostDraw(const GameContext& gameContext)
{
	XMFLOAT4X4 vp = gameContext.pCamera->GetViewProjection();
	m_pWvpVariable->SetMatrix(reinterpret_cast<float*>(&vp));
	XMFLOAT4X4 vi = gameContext.pCamera->GetViewInverse();
	m_pViewInverseVariable->SetMatrix(reinterpret_cast<float*>(&vi));
	m_pTextureVariable->SetResource(m_pParticleTexture->GetShaderResourceView());

	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT offset = 0;
	UINT stride = sizeof(ParticleVertex);
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	D3DX11_TECHNIQUE_DESC techDesc;
	m_pDefaultTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p) {
		m_pDefaultTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->Draw(m_ActiveParticles, 0);
	}
}

void ParticleEmitterComponent::SetEnabled(bool enabled)
{
	m_Enabled = enabled;
}

void ParticleEmitterComponent::Burst(const GameContext &gameContext)
{
	auto avgEnergy = m_Settings.MinEnergy + (m_Settings.MaxEnergy - m_Settings.MinEnergy / 2.f);
	auto particleInterval = avgEnergy / m_ParticleCount;

	m_LastParticleInit += gameContext.pGameTime->GetElapsed();

	m_ActiveParticles = m_ParticleCount;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	auto hr = gameContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	Logger::LogHResult(hr, L"ParticleEmitterComponent::Update > Map");

	ParticleVertex* pBuffer = (ParticleVertex*)mappedResource.pData;

	for each(auto particle in m_Particles)
	{
		particle->Update(gameContext);

		if (!particle->IsActive()) {
			particle->Init(GetTransform()->GetWorldPosition());

			auto vertInfo = particle->GetVertexInfo();
			auto bufData = (pBuffer + m_ActiveParticles);
			(*bufData) = vertInfo;

			m_LastParticleInit = 0;
		}
	}

	gameContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
}
