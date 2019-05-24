#include "stdafx.h"
#include "Player.h"
#include "PlayingField.h"
#include "Components\Components.h"
#include "Prefabs\Prefabs.h"
#include "Base\OverlordGame.h"
#include "Scenegraph\SceneManager.h"
#include "Graphics\ModelAnimator.h"
#include "Components/ParticleEmitterComponent.h"
#include "Base\SoundManager.h"
#include <algorithm>



Player::Player(PlayingField* field, XMINT2 spawnPos, ControlIDs controls, int materialID, const GameContext& gameContext) : m_PlayingField(field), m_GridPos(spawnPos), m_Controls(controls)
{
	m_pFMODSystem = SoundManager::GetInstance()->GetSystem();
	FMOD_RESULT res = m_pFMODSystem->createSound("Resources/Sounds/BMAN_Running.wav", FMOD_LOOP_NORMAL, 0, &m_pSoundFootsteps);
	SoundManager::ErrorCheck(res);
	res = m_pFMODSystem->playSound(m_pSoundFootsteps, 0, false, &m_pRunningChannel);
	SoundManager::ErrorCheck(res);
	m_pRunningChannel->setPaused(true);


	XMFLOAT2 pos = m_PlayingField->GetCellWorldPos(spawnPos);
	XMFLOAT3 newPos = XMFLOAT3(pos.x, 0, pos.y);
	GetTransform()->Translate(newPos);

	auto pMesh = new ModelComponent(L"./Resources/Meshes/BMAN_Char.ovm");
	pMesh->SetMaterial(materialID);
	m_pPlayer = new GameObject();
	m_pPlayer->AddComponent(pMesh);

	AddChild(m_pPlayer);
	m_pPlayer->GetTransform()->Scale({ 0.03f, 0.03f , 0.03f });
	m_pPlayer->GetTransform()->Translate(XMFLOAT3(0, -0.5f, 0));
	m_pPlayer->GetTransform()->Rotate(XMFLOAT3(0, m_CurrentRot, 0));

	m_RunParticleEmitter = new ParticleEmitterComponent(L"./Resources/Textures/BMAN_Dust_Temp.png", 50);
	m_RunParticleEmitter->SetVelocity(XMFLOAT3(0, 0, 0));

	m_RunParticleEmitter->SetMinSize(0.2f);
	m_RunParticleEmitter->SetMaxSize(0.3f);
	m_RunParticleEmitter->SetMinEnergy(0.3f);
	m_RunParticleEmitter->SetMaxEnergy(0.5f);
	m_RunParticleEmitter->SetMinSizeGrow(-1);
	m_RunParticleEmitter->SetMaxSizeGrow(-1);
	m_RunParticleEmitter->SetMinEmitterRange(0.0f);
	m_RunParticleEmitter->SetMaxEmitterRange(0.2f);
	m_RunParticleEmitter->SetColor(XMFLOAT4(0.9f, 0.9f, 0.9f, 1));
	m_RunParticleEmitter->SetSpawnShape(EmitterSpawnShape::CIRCLE_Y);
	m_RunParticleEmitter->SetEnabled(false);
	GameObject* pEmitter = new GameObject();
	pEmitter->AddComponent(m_RunParticleEmitter);
	m_pPlayer->AddChild(pEmitter);
}

Player::~Player()
{
	m_pRunningChannel->stop();
	m_pSoundFootsteps->release();
}

void Player::DisableControls(bool disabled)
{
	m_Disabled = disabled;
}

void Player::Update(const GameContext & gameContext)
{

	if (m_Init) {
		m_Init = false;
		m_pPlayer->GetComponent<ModelComponent>()->GetAnimator()->SetAnimation(3);
	}

	float test = LerpAngle(10, 350, 1);
	if (m_State == PlayerState::DEAD || m_State == PlayerState::VICTORY)
	{
		return;
	}

	bool running = false;
	if (!m_Disabled) {

		if (gameContext.pInput->IsActionTriggered(m_Controls.RIGHT)) { //R
			m_TargetRot = 270;
			Move({ 1, 0 }, gameContext);
			running = true;
		}


		if (gameContext.pInput->IsActionTriggered(m_Controls.LEFT)) { //L
			m_TargetRot = running ? LerpAngle(90, m_TargetRot, 0.5f) : 90;
			Move({ -1, 0 }, gameContext);
			running = true;
		}

		if (gameContext.pInput->IsActionTriggered(m_Controls.FORWARD)) { //U
			m_TargetRot = running ? LerpAngle(180, m_TargetRot, 0.5f) : 180;
			Move({ 0, 1 }, gameContext);
			running = true;
		}

		if (gameContext.pInput->IsActionTriggered(m_Controls.BACKWARD)) { //D
			m_TargetRot = running ? LerpAngle(0, m_TargetRot, 0.5f) : 0;
			Move({ 0, -1 }, gameContext);
			running = true;
		}

		if (gameContext.pInput->IsActionTriggered(m_Controls.BOMB) && m_BombCount > 0) {
			XMFLOAT2 worldPos;
			worldPos.x = GetTransform()->GetPosition().x;
			worldPos.y = GetTransform()->GetPosition().z;
			m_PlayingField->PlaceBomb(m_BombPower, m_PlayingField->GetCellCoord(worldPos), 5, this, gameContext);
		}
	}
	

	if (running) {
		SwitchState(PlayerState::RUNNING, gameContext);
	}
	else {
		SwitchState(PlayerState::IDLE, gameContext);
	}

	m_CurrentRot = LerpAngle(m_CurrentRot, m_TargetRot, 10.0f * gameContext.pGameTime->GetElapsed());
	m_pPlayer->GetTransform()->Rotate(XMFLOAT3(0, m_CurrentRot, 0));


}

void Player::Kill(const GameContext& gameContext)
{
	SwitchState(PlayerState::DEAD, gameContext);
}

void Player::Dance(const GameContext& gameContext)
{
	SwitchState(PlayerState::VICTORY, gameContext);
}

void Player::IncreaseSpeed()
{
	++m_MovementSpeed;
	if (m_State == PlayerState::RUNNING) m_pPlayer->GetComponent<ModelComponent>()->GetAnimator()->SetAnimationSpeed(m_MovementSpeed / 3.0f);
}

void Player::IncreaseBombPower()
{
	++m_BombPower;
}

void Player::IncreaseBombCount()
{
	m_BombCount++;
}

void Player::DecreaseBombCount()
{
	m_BombCount--;
}

float Player::LerpAngle(float from, float to, float amount) const
{
	return from + ShortestAngleDist(from, to) * amount;
}

float Player::ShortestAngleDist(float a0, float a1) const
{
	float max = 360;
	float da = fmod(a1 - a0, max);
	return fmod(2 * da, max) - da;
}

void Player::SwitchState(const PlayerState &state, const GameContext &gameContext)
{
	if (state == m_State) return;

	switch (state) {
	case Player::PlayerState::IDLE:
		if (m_State == PlayerState::RUNNING) {
			m_State = PlayerState::IDLE;
			m_pPlayer->GetComponent<ModelComponent>()->GetAnimator()->SetAnimation(3);
			m_pPlayer->GetComponent<ModelComponent>()->GetAnimator()->SetAnimationSpeed(1);
			m_RunParticleEmitter->SetEnabled(false);
			m_pRunningChannel->setPaused(true);
		}
		break;
	case Player::PlayerState::RUNNING:
		if (m_State == PlayerState::IDLE) {
			m_State = PlayerState::RUNNING;
			m_pPlayer->GetComponent<ModelComponent>()->GetAnimator()->SetAnimation(1);
			m_pPlayer->GetComponent<ModelComponent>()->GetAnimator()->SetAnimationSpeed(m_MovementSpeed / 3.0f);
			m_RunParticleEmitter->SetEnabled(true);
			m_pRunningChannel->setPaused(false);
		}
		break;
	case Player::PlayerState::DEAD:
		if (m_State != PlayerState::VICTORY)
		{
			m_State = PlayerState::DEAD;
			m_pPlayer->GetComponent<ModelComponent>()->GetAnimator()->SetAnimation(0);
			m_pPlayer->GetComponent<ModelComponent>()->GetAnimator()->SetAnimationSpeed(1);
			m_pPlayer->GetComponent<ModelComponent>()->GetAnimator()->SetLoop(false);
			m_RunParticleEmitter->SetEnabled(false);
			m_pRunningChannel->setPaused(true);
		}
		break;
	case Player::PlayerState::VICTORY:
		if (m_State != PlayerState::DEAD)
		{
			m_State = PlayerState::VICTORY;
			m_pPlayer->GetComponent<ModelComponent>()->GetAnimator()->SetAnimation(2);
			m_pPlayer->GetComponent<ModelComponent>()->GetAnimator()->SetAnimationSpeed(1);
			m_RunParticleEmitter->SetEnabled(false);
			m_pRunningChannel->setPaused(true);
		}
		break;
	default:
		break;
	}
}

void Player::Move(XMFLOAT2 dir, const GameContext & gameContext)
{
	float offset = (m_PlayerWidth * 0.5f);
	XMFLOAT3 newPos3 = GetTransform()->GetPosition();
	XMFLOAT2 currPos;
	XMFLOAT2 newPos;

	currPos.x = newPos3.x;
	currPos.y = newPos3.z;
	currPos.x += dir.x * offset;
	currPos.y += dir.y * offset;

	newPos.x = newPos3.x;
	newPos.y = newPos3.z;
	newPos.x += dir.x * (offset + m_MovementSpeed * gameContext.pGameTime->GetElapsed());
	newPos.y += dir.y * (offset + m_MovementSpeed * gameContext.pGameTime->GetElapsed());

	XMFLOAT2 currC1 = currPos;
	XMFLOAT2 currC2 = currPos;

	XMFLOAT2 newC1 = newPos;
	XMFLOAT2 newC2 = newPos;

	if (dir.x != 0) {
		currC1.y += offset;
		currC2.y -= offset;

		newC1.y += offset;
		newC2.y -= offset;
	}
	else {
		currC1.x += offset;
		currC2.x -= offset;

		newC1.x += offset;
		newC2.x -= offset;
	}

	PlayingField::CellType cCell, c1Cell, c2Cell;
	cCell = m_PlayingField->GetCellFromWorldPos(newPos);
	c1Cell = m_PlayingField->GetCellFromWorldPos(newC1);
	c2Cell = m_PlayingField->GetCellFromWorldPos(newC2);


	//if (!(m_PlayingField->GetCellFromWorldPos(currPos) == PlayingField::CellType::BOMB ||
	//	m_PlayingField->GetCellFromWorldPos(currC1) == PlayingField::CellType::BOMB ||
	//	m_PlayingField->GetCellFromWorldPos(currC2) == PlayingField::CellType::BOMB)) {
	//	if (cCell != PlayingField::CellType::EMPTY) return;
	//	if (c1Cell != PlayingField::CellType::EMPTY) return;
	//	if (c2Cell != PlayingField::CellType::EMPTY) return;
	//}

	if (cCell == PlayingField::CellType::BOMB)
	{
		XMINT2 cCoord = m_PlayingField->GetCellCoord(newPos);
		XMINT2 playerCoord = m_PlayingField->GetCellCoord(currPos);
		if (!(cCoord == playerCoord))
		{
			return;
		}
		else
		{
			if (c1Cell != PlayingField::CellType::EMPTY && c2Cell == PlayingField::CellType::EMPTY)
			{
				if (dir.x != 0)
				{
					dir.x /= 2;
					dir.y = -0.5f;
				}
				else
				{
					dir.y /= 2.0f;
					dir.x = -0.5f;
				}

				newPos3.x += dir.x * m_MovementSpeed * gameContext.pGameTime->GetElapsed();
				newPos3.z += dir.y * m_MovementSpeed * gameContext.pGameTime->GetElapsed();
				GetTransform()->Translate(newPos3);
			}
			else if (c2Cell != PlayingField::CellType::EMPTY && c1Cell == PlayingField::CellType::EMPTY)
			{
				if (dir.x != 0)
				{
					dir.x /= 2;
					dir.y = 0.5f;
				}
				else
				{
					dir.y /= 2.0f;
					dir.x = 0.5f;
				}

				newPos3.x += dir.x * m_MovementSpeed * gameContext.pGameTime->GetElapsed();
				newPos3.z += dir.y * m_MovementSpeed * gameContext.pGameTime->GetElapsed();
				GetTransform()->Translate(newPos3);
			}
			else
			{
				newPos3.x += dir.x * m_MovementSpeed * gameContext.pGameTime->GetElapsed();
				newPos3.z += dir.y * m_MovementSpeed * gameContext.pGameTime->GetElapsed();
				GetTransform()->Translate(newPos3);
			}
		}
	}
	else
	{
		if (c1Cell != PlayingField::CellType::EMPTY && c2Cell == PlayingField::CellType::EMPTY && cCell == PlayingField::CellType::EMPTY)
		{
			if (dir.x != 0)
			{
				dir.x /= 2;
				dir.y = -0.5f;
			}
			else
			{
				dir.y /= 2.0f;
				dir.x = -0.5f;
			}

			newPos3.x += dir.x * m_MovementSpeed * gameContext.pGameTime->GetElapsed();
			newPos3.z += dir.y * m_MovementSpeed * gameContext.pGameTime->GetElapsed();
			GetTransform()->Translate(newPos3);
		}
		else if (c2Cell != PlayingField::CellType::EMPTY && c1Cell == PlayingField::CellType::EMPTY && cCell == PlayingField::CellType::EMPTY)
		{
			if (dir.x != 0)
			{
				dir.x /= 2;
				dir.y = 0.5f;
			}
			else
			{
				dir.y /= 2.0f;
				dir.x = 0.5f;
			}

			newPos3.x += dir.x * m_MovementSpeed * gameContext.pGameTime->GetElapsed();
			newPos3.z += dir.y * m_MovementSpeed * gameContext.pGameTime->GetElapsed();
			GetTransform()->Translate(newPos3);
		}
		else if (cCell == PlayingField::CellType::EMPTY)
		{
			newPos3.x += dir.x * m_MovementSpeed * gameContext.pGameTime->GetElapsed();
			newPos3.z += dir.y * m_MovementSpeed * gameContext.pGameTime->GetElapsed();
			GetTransform()->Translate(newPos3);
		}
	}

	
}