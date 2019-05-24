#pragma once
#include "Scenegraph\GameObject.h"
class Player;
class PlayingField : public GameObject
{
public:
	PlayingField(XMINT2 size, const GameContext& gameContext);
	~PlayingField();

	enum class CellType {
		EMPTY,
		WALL,
		BLOCK,
		BOMB,
	};	

	void Update(const GameContext& gameContext);

	void AddPlayer1(Player* p);
	void AddPlayer2(Player* p);
	CellType GetCellFromCoord(const XMINT2 &cellCoord) const;
	CellType GetCellFromWorldPos(const XMFLOAT2 &worldPos) const;
	XMFLOAT2 GetCellWorldPos(const XMINT2 &cellCoord) const;
	XMINT2 GetCellCoord(const XMFLOAT2 &worldPos) const;
	bool IsSolid(const XMINT2 &coord) const;

	void PlaceBomb(int explosionDistance, const XMINT2 &pos, float time, Player* player, const GameContext &gameContext);

private:
	struct Bomb
	{
		int explosionDistance;
		XMINT2 pos;
		float timer;
		GameObject* pBomb;
		Player* pPlayer;
	};
	
	enum class PickupType {
		SPEED,
		BOMBPOWER,
		BOMBAMOUNT,
	};

	void SetCell(const XMINT2 &coord, const CellType &type);
	void DestroyWallAtCoord(const XMINT2 &coord);
	void UpdateBombs(const GameContext &gameContext);
	void DetonateBomb(Bomb& bomb, int &index, const GameContext &gameContext);
	void UpdateEmitters(const GameContext &gameContext);
	void UpdatePickups(const GameContext &gameContext);
	
private:
	Player* m_pPlayer1 = nullptr;
	Player* m_pPlayer2 = nullptr;
	XMINT2 m_Size;
	XMFLOAT2 m_Origin;
	float m_CellSize = 1.0f;

	std::vector<CellType> m_Cells;
	std::vector<Bomb> m_Bombs;
	std::vector<std::pair<float, GameObject*>> m_Emitters;
	std::vector<std::pair<PickupType, GameObject*>> m_Pickups;
	std::map<int, GameObject*> m_pBlocks;
	
	FMOD::System *m_pFMODSystem;

	FMOD::Channel *m_pAmbientChannel;
	FMOD::Sound *m_pSoundWaves;

	FMOD::Channel *m_pSFXChannel;
	FMOD::Sound *m_pSoundPlop;
	FMOD::Sound *m_pSoundExplosion;
	FMOD::Sound *m_pSoundPickup;
private:
	PlayingField(const PlayingField& ref) = delete;
	PlayingField& operator=(const PlayingField& ref) = delete;
};