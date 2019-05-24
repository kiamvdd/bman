#pragma once
class PlayingField;
class ParticleEmitterComponent;
#include "Scenegraph\GameObject.h"

struct ControlIDs {
	int RIGHT, LEFT, FORWARD, BACKWARD, BOMB;
};

class Player : public GameObject 
{
private:
	enum class PlayerState {
		IDLE,
		RUNNING,
		DEAD,
		VICTORY,
	};

public:
	Player(PlayingField* field, XMINT2 spawnPos, ControlIDs controls, int materialID, const GameContext& gameContext);
	~Player();

	void DisableControls(bool disabled);
	void Update(const GameContext& gameContext);
	void Kill(const GameContext& gameContext);
	void Dance(const GameContext& gameContext);
	void IncreaseSpeed();
	void IncreaseBombPower();
	void IncreaseBombCount();
	void DecreaseBombCount();
	bool IsAlive() { return m_State != PlayerState::DEAD; };
private:


	float LerpAngle(float from, float to, float amount) const;
	float ShortestAngleDist(float a0, float a1) const;
	void SwitchState(const PlayerState &state, const GameContext &gameContext);

	PlayerState m_State = PlayerState::IDLE;

	int m_BombCount = 1;
	ControlIDs m_Controls;
	void Move(XMFLOAT2 dir, const GameContext& gameContext);
	bool m_IsMoving = false;
	float m_PlayerWidth = 0.5f;
	XMINT2 m_GridPos;
	float m_TargetRot = 90;
	float m_CurrentRot = 0;
	float speed = 1;
	float m_MovementSpeed = 3;
	GameObject* m_pPlayer = nullptr;
	PlayingField* m_PlayingField;
	bool m_Init = true;
	bool m_Disabled = false;

	int m_BombPower = 1;

	ParticleEmitterComponent* m_RunParticleEmitter = nullptr;

	FMOD::System *m_pFMODSystem;

	FMOD::Channel *m_pRunningChannel;
	FMOD::Sound *m_pSoundFootsteps;
private:
	Player(const Player& ref) = delete;
	Player& operator=(const Player& ref) = delete;
};