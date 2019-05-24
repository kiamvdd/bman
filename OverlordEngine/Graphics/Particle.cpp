//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Particle.h"
#include "../Components/TransformComponent.h"
#include "../Helpers/MathHelper.h"


Particle::Particle(const ParticleEmitterSettings& emitterSettings) :
	m_EmitterSettings(emitterSettings)
{
}


Particle::~Particle(void)
{
}

void Particle::Update(const GameContext& gameContext)
{
	if (!m_IsActive) {
		return;
	}

	m_CurrentEnergy -= gameContext.pGameTime->GetElapsed();
	if (m_CurrentEnergy <= 0) {
		m_IsActive = false;
		return;
	}
	
	// Velocity
	XMFLOAT3 v = m_EmitterSettings.SpawnBasedVelocityEnabled ? m_SpawnBasedVelocity : m_EmitterSettings.Velocity;
	float dTime = gameContext.pGameTime->GetElapsed();
	v.x *= dTime;
	v.y *= dTime;
	v.z *= dTime;
	m_VertexInfo.Position.x += v.x;
	m_VertexInfo.Position.y += v.y;
	m_VertexInfo.Position.z += v.z;

	// Color
	m_VertexInfo.Color = m_EmitterSettings.Color;
	float particleLifePercent = m_CurrentEnergy / m_TotalEnergy;
	m_VertexInfo.Color.w = particleLifePercent * 2;

	// Size
	m_VertexInfo.Size = m_InitSize + (m_InitSize * m_SizeGrow * (1 - particleLifePercent));
}

void Particle::Init(XMFLOAT3 initPosition)
{
	m_IsActive = true;
	m_CurrentEnergy = randF(m_EmitterSettings.MinEnergy, m_EmitterSettings.MaxEnergy);
	m_TotalEnergy = m_CurrentEnergy;

	XMFLOAT3 newPosition;
	XMVECTOR randVec = { 1, 0, 0 };
	switch (m_EmitterSettings.Shape)
	{
	case EmitterSpawnShape::SPHERE:
		{
			XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI));
			randVec = XMVector3TransformNormal(randVec, rotMat);

			float randDist = randF(m_EmitterSettings.MinEmitterRange, m_EmitterSettings.MaxEmitterRange);
			randVec = randVec * randDist;
			XMVECTOR pos = XMLoadFloat3(&initPosition);
			XMStoreFloat3(&newPosition, randVec + pos);
		}
		break;
	case EmitterSpawnShape::CIRCLE_X:
		{
			randVec = { 0, 1, 0 };
			XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(randF(-XM_PI, XM_PI), 0, 0);
			randVec = XMVector3TransformNormal(randVec, rotMat);

			float randDist = randF(m_EmitterSettings.MinEmitterRange, m_EmitterSettings.MaxEmitterRange);
			randVec = randVec * randDist;
			XMVECTOR pos = XMLoadFloat3(&initPosition);
			XMStoreFloat3(&newPosition, randVec + pos);
		}
		break;
	case EmitterSpawnShape::CIRCLE_Y:
		{
			XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(0, randF(-XM_PI, XM_PI), 0);
			randVec = XMVector3TransformNormal(randVec, rotMat);

			float randDist = randF(m_EmitterSettings.MinEmitterRange, m_EmitterSettings.MaxEmitterRange);
			randVec = randVec * randDist;
			XMVECTOR pos = XMLoadFloat3(&initPosition);
			XMStoreFloat3(&newPosition, randVec + pos);
		}
		break;
	case EmitterSpawnShape::CIRCLE_Z:
		{
			XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(0, 0, randF(-XM_PI, XM_PI));
			randVec = XMVector3TransformNormal(randVec, rotMat);

			float randDist = randF(m_EmitterSettings.MinEmitterRange, m_EmitterSettings.MaxEmitterRange);
			randVec = randVec * randDist;
			XMVECTOR pos = XMLoadFloat3(&initPosition);
			XMStoreFloat3(&newPosition, randVec + pos);
		}
		break;
	case EmitterSpawnShape::CUBE:
		int randX, randY, randZ;
		randX = (rand() % 2 == 0) ? -1 : 1;
		randY = (rand() % 2 == 0) ? -1 : 1;
		randZ = (rand() % 2 == 0) ? -1 : 1;

		randVec = { randF(m_EmitterSettings.MinEmitterRange, m_EmitterSettings.MaxEmitterRange) * randX, randF(m_EmitterSettings.MinEmitterRange, m_EmitterSettings.MaxEmitterRange) * randY, randF(m_EmitterSettings.MinEmitterRange, m_EmitterSettings.MaxEmitterRange) * randZ };
		XMVECTOR pos = XMLoadFloat3(&initPosition);
		XMStoreFloat3(&newPosition, randVec + pos);
		break;
	default:
		break;
	}


	m_VertexInfo.Position = newPosition;

	m_VertexInfo.Size = randF(m_EmitterSettings.MinSize, m_EmitterSettings.MaxSize);
	m_InitSize = m_VertexInfo.Size;
	m_SizeGrow = randF(m_EmitterSettings.MinSizeGrow, m_EmitterSettings.MaxSizeGrow);
	m_VertexInfo.Rotation = randF(-XM_PI, XM_PI);

	if (m_EmitterSettings.SpawnBasedVelocityEnabled)
	{
		randVec = XMVector3Normalize(randVec);
		randVec *= randF(m_EmitterSettings.MinSpawnBasedVelocity, m_EmitterSettings.MaxSpawnBasedVelocity);
		XMStoreFloat3(&m_SpawnBasedVelocity, randVec);
	}
}