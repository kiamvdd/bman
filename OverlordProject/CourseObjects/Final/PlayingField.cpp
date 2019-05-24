#include "stdafx.h"
#include "PlayingField.h"
#include "Player.h"
#include "Prefabs\Prefabs.h"
#include "Components\TransformComponent.h"
#include "Components\ModelComponent.h"
#include "Base\SoundManager.h"
#include "Components/ParticleEmitterComponent.h"

PlayingField::PlayingField(XMINT2 size, const GameContext& gameContext) : m_Size(size)
{
	m_pFMODSystem = SoundManager::GetInstance()->GetSystem();
	FMOD_RESULT res = m_pFMODSystem->createStream("Resources/Sounds/BMAN_Waves.wav", FMOD_LOOP_NORMAL, 0, &m_pSoundWaves);
	SoundManager::ErrorCheck(res);

	res = m_pFMODSystem->createSound("Resources/Sounds/BMAN_Plop_Short.wav", FMOD_DEFAULT, 0, &m_pSoundPlop);
	SoundManager::ErrorCheck(res);

	res = m_pFMODSystem->createSound("Resources/Sounds/BMAN_Explosion.wav", FMOD_DEFAULT, 0, &m_pSoundExplosion);
	SoundManager::ErrorCheck(res);

	res = m_pFMODSystem->createSound("Resources/Sounds/BMAN_Pickup.wav", FMOD_DEFAULT, 0, &m_pSoundPickup);
	SoundManager::ErrorCheck(res);

	res = m_pFMODSystem->playSound(m_pSoundWaves, 0, false, &m_pAmbientChannel);
	SoundManager::ErrorCheck(res);
	m_pAmbientChannel->setVolume(0.2f);

	res = m_pFMODSystem->playSound(m_pSoundPlop, 0, true, &m_pSFXChannel);
	SoundManager::ErrorCheck(res);
	m_pSFXChannel->stop();

	m_Cells.resize(size.x * size.y);
	m_Origin.x = -m_Size.x * m_CellSize / 2.0f;
	m_Origin.y = m_Size.y * m_CellSize / 2.0f;
	m_Origin.x += m_CellSize / 2.0f;
	m_Origin.y -= m_CellSize / 2.0f;

	// Generate playing field



	// ****************************************************************************
	auto pMesh = new ModelComponent(L"Resources/Meshes/BMAN_Plane.ovm");
	pMesh->SetMaterial(5);
	auto pWater = new GameObject();
	pWater->AddComponent(pMesh);
	AddChild(pWater);
	pWater->GetTransform()->Scale({ 0.11f, 0.11f , 0.11f });
	pWater->GetTransform()->Rotate(XMFLOAT3(90, 0, 0));
	pWater->GetTransform()->Translate(XMFLOAT3(-14, -1.6f, 0));

	pWater = new GameObject();
	pMesh = new ModelComponent(L"Resources/Meshes/BMAN_Plane.ovm");
	pMesh->SetMaterial(5);
	pWater->AddComponent(pMesh);
	AddChild(pWater);
	pWater->GetTransform()->Scale({ 0.11f, 0.11f , 0.11f });
	pWater->GetTransform()->Rotate(XMFLOAT3(90, 0, 0));
	pWater->GetTransform()->Translate(XMFLOAT3(-14, -1.6f, 11));

	pWater = new GameObject();
	pMesh = new ModelComponent(L"Resources/Meshes/BMAN_Plane.ovm");
	pMesh->SetMaterial(5);
	pWater->AddComponent(pMesh);
	AddChild(pWater);
	pWater->GetTransform()->Scale({ 0.11f, 0.11f , 0.11f });
	pWater->GetTransform()->Rotate(XMFLOAT3(90, 0, 0));
	pWater->GetTransform()->Translate(XMFLOAT3(-14, -1.6f, -11));

	// ****************************************************************************
	pMesh = new ModelComponent(L"Resources/Meshes/BMAN_Plane.ovm");
	pMesh->SetMaterial(5);
	pWater = new GameObject();
	pWater->AddComponent(pMesh);
	AddChild(pWater);
	pWater->GetTransform()->Scale({ 0.11f, 0.11f , 0.11f });
	pWater->GetTransform()->Rotate(XMFLOAT3(90, 0, 0));
	pWater->GetTransform()->Translate(XMFLOAT3(-3, -1.6f, 0));

	pWater = new GameObject();
	pMesh = new ModelComponent(L"Resources/Meshes/BMAN_Plane.ovm");
	pMesh->SetMaterial(5);
	pWater->AddComponent(pMesh);
	AddChild(pWater);
	pWater->GetTransform()->Scale({ 0.11f, 0.11f , 0.11f });
	pWater->GetTransform()->Rotate(XMFLOAT3(90, 0, 0));
	pWater->GetTransform()->Translate(XMFLOAT3(-3, -1.6f, 11));

	pWater = new GameObject();
	pMesh = new ModelComponent(L"Resources/Meshes/BMAN_Plane.ovm");
	pMesh->SetMaterial(5);
	pWater->AddComponent(pMesh);
	AddChild(pWater);
	pWater->GetTransform()->Scale({ 0.11f, 0.11f , 0.11f });
	pWater->GetTransform()->Rotate(XMFLOAT3(90, 0, 0));
	pWater->GetTransform()->Translate(XMFLOAT3(-3, -1.6f, -11));


	// ****************************************************************************
	pMesh = new ModelComponent(L"Resources/Meshes/BMAN_Plane.ovm");
	pMesh->SetMaterial(5);
	pWater = new GameObject();
	pWater->AddComponent(pMesh);
	AddChild(pWater);
	pWater->GetTransform()->Scale({ 0.11f, 0.11f , 0.11f });
	pWater->GetTransform()->Rotate(XMFLOAT3(90, 0, 0));
	pWater->GetTransform()->Translate(XMFLOAT3(8, -1.6f, 0));

	pWater = new GameObject();
	pMesh = new ModelComponent(L"Resources/Meshes/BMAN_Plane.ovm");
	pMesh->SetMaterial(5);
	pWater->AddComponent(pMesh);
	AddChild(pWater);
	pWater->GetTransform()->Scale({ 0.11f, 0.11f , 0.11f });
	pWater->GetTransform()->Rotate(XMFLOAT3(90, 0, 0));
	pWater->GetTransform()->Translate(XMFLOAT3(8, -1.6f, 11));

	pWater = new GameObject();
	pMesh = new ModelComponent(L"Resources/Meshes/BMAN_Plane.ovm");
	pMesh->SetMaterial(5);
	pWater->AddComponent(pMesh);
	AddChild(pWater);
	pWater->GetTransform()->Scale({ 0.11f, 0.11f , 0.11f });
	pWater->GetTransform()->Rotate(XMFLOAT3(90, 0, 0));
	pWater->GetTransform()->Translate(XMFLOAT3(8, -1.6f, -11));

	auto pFoam = new GameObject();
	pMesh = new ModelComponent(L"Resources/Meshes/BMAN_Foam.ovm");
	pMesh->SetMaterial(6);
	pFoam->AddComponent(pMesh);
	AddChild(pFoam);
	pFoam->GetTransform()->Scale({ 0.1f, 0.1f , 0.1f });
	pFoam->GetTransform()->Rotate(XMFLOAT3(90, 90, 0));
	pFoam->GetTransform()->Translate(XMFLOAT3(0, -1.4f, 0));

	for (int i = 0; i < m_Cells.size(); i++) {
		int x = i % size.x;
		int y = i / size.x;

		if (x == 0 || x == size.x - 1 || y == 0 || y == size.y - 1) {
			//auto pModel = new ModelComponent(L"Resources/Meshes/BMAN_Wall3.ovm");
			//pModel->SetMaterial(0);
			//auto pWall = new GameObject();
			//AddChild(pWall);
			//pWall->AddComponent(pModel);
			//pWall->GetTransform()->Scale({ 0.01f, 0.01f , 0.01f });
			//pWall->GetTransform()->Rotate(XMFLOAT3(90, 0, 0));

			//pWall->GetTransform()->Translate(XMFLOAT3(x * m_CellSize + m_Origin.x, -0.5f, (y * -m_CellSize + m_Origin.y)));
			m_Cells[i] = CellType::WALL;
		} else {
			if (x % 2 == 0 && y % 2 == 0) {
				//auto pModel = new ModelComponent(L"Resources/Meshes/BMAN_Wall3.ovm");
				//pModel->SetMaterial(2);
				//auto pWall = new GameObject();
				//AddChild(pWall);
				//pWall->AddComponent(pModel);
				//pWall->GetTransform()->Scale({ 0.01f, 0.01f , 0.01f });
				//int r = rand() % 2;
				//pWall->GetTransform()->Rotate(XMFLOAT3(90, r == 0 ? 0 : 90, 0));

				//pWall->GetTransform()->Translate(XMFLOAT3(x * m_CellSize + m_Origin.x, -0.5f, (y * -m_CellSize + m_Origin.y)));
				m_Cells[i] = CellType::WALL;
			} else {
				auto pModel = new ModelComponent(L"Resources/Meshes/BMAN_Box.ovm");
				pModel->SetMaterial(4);
				auto pFloor = new GameObject();
				AddChild(pFloor);
				pFloor->AddComponent(pModel);
				pFloor->GetTransform()->Scale({ 0.05f, 0.05f , 0.05f });
				pFloor->GetTransform()->Rotate(XMFLOAT3(90, 0, 0));
				pFloor->GetTransform()->Translate(XMFLOAT3(x * m_CellSize + m_Origin.x, -1.5f, (y * -m_CellSize + m_Origin.y)));


				int r = rand() % 6;

				bool b1 = (x > 2 && x < m_Size.x - 3);
				bool b2 = (y > 2 && y < m_Size.y - 3);

				if (r != 0 && (b1 || b2)) {
					m_Cells[i] = CellType::BLOCK;
					auto pModel = new ModelComponent(L"Resources/Meshes/BMAN_Box.ovm");
					pModel->SetMaterial(3);
					auto pBox = new GameObject();
					AddChild(pBox);
					pBox->AddComponent(pModel);
					pBox->GetTransform()->Scale({ 0.05f, 0.05f , 0.05f });
					int rnd = rand() % 2;
					pBox->GetTransform()->Rotate(XMFLOAT3(90, rnd == 0 ? 0 : 90, 0));

					pBox->GetTransform()->Translate(XMFLOAT3(x * m_CellSize + m_Origin.x, -0.6f, (y * -m_CellSize + m_Origin.y)));
					m_pBlocks.insert(std::pair<int, GameObject*>(i, pBox));

					int r2 = rand() % 6;
					if (r2 == 0) {
						r2 = rand() % 3;
						PickupType type;
						switch (r2) {
						case 0:
							type = PickupType::BOMBPOWER;
							break;
						case 1:
							type = PickupType::BOMBAMOUNT;
							break;
						case 2:
							type = PickupType::SPEED;
							break;
						default:
							break;
						}

						auto pPickupMesh = new ModelComponent(L"Resources/Meshes/BMAN_Pickup.ovm");
						pPickupMesh->SetMaterial(20 + r2);
						auto pPickup = new GameObject();
						AddChild(pPickup);
						pPickup->AddComponent(pPickupMesh);
						pPickup->GetTransform()->Translate(XMFLOAT3(x * m_CellSize + m_Origin.x, 0, (y * -m_CellSize + m_Origin.y)));
						pPickup->GetTransform()->Rotate(XMFLOAT3(75, 0, 0));
						pPickup->GetTransform()->Scale(XMFLOAT3(0.009f, 0.009f, 0.009f));
						m_Pickups.push_back(std::pair<PickupType, GameObject*>(type, pPickup));
					}
				} else {
					m_Cells[i] = CellType::EMPTY;
				}
			}
		}
	}
}

PlayingField::~PlayingField()
{
	m_pAmbientChannel->stop();
	m_pSFXChannel->stop();

	m_pSoundWaves->release();
	m_pSoundExplosion->release();
	m_pSoundPlop->release();
}

void PlayingField::Update(const GameContext & gameContext)
{
	UpdateBombs(gameContext);
	UpdateEmitters(gameContext);
	UpdatePickups(gameContext);
}

void PlayingField::AddPlayer1(Player* p)
{
	m_pPlayer1 = p;
}

void PlayingField::AddPlayer2(Player * p)
{
	m_pPlayer2 = p;
}

PlayingField::CellType PlayingField::GetCellFromCoord(const XMINT2 &cellCoord) const
{
	XMINT2 clampedCoord = cellCoord;

	if (clampedCoord.x < 0) clampedCoord.x = 0;
	if (clampedCoord.x > m_Size.x - 1) clampedCoord.x = m_Size.x - 1;

	if (clampedCoord.y < 0) clampedCoord.y = 0;
	if (clampedCoord.y > m_Size.y - 1) clampedCoord.y = m_Size.y - 1;

	return m_Cells[clampedCoord.y * m_Size.x + clampedCoord.x];
}

XMFLOAT2 PlayingField::GetCellWorldPos(const XMINT2 &cellCoord) const
{
	XMFLOAT2 worldPos;
	worldPos.x = (cellCoord.x * m_CellSize) + (m_Origin.x);
	worldPos.y = (-cellCoord.y * m_CellSize) + (m_Origin.y);
	return worldPos;
}

XMINT2 PlayingField::GetCellCoord(const XMFLOAT2 &worldPos) const
{
	XMINT2 result;
	result.x = (worldPos.x - (m_Origin.x - m_CellSize / 2.0f)) / m_CellSize;
	result.y = -(worldPos.y - (m_Origin.y + m_CellSize / 2.0f)) / m_CellSize;

	if (result.x < 0) result.x = 0;
	if (result.x > m_Size.x - 1) result.x = m_Size.x - 1;

	if (result.y < 0) result.y = 0;
	if (result.y > m_Size.y - 1) result.y = m_Size.y - 1;

	return result;
}

bool PlayingField::IsSolid(const XMINT2 & coord) const
{
	return !(GetCellFromCoord(coord) == CellType::EMPTY);
}

void PlayingField::PlaceBomb(int explosionDistance, const XMINT2 & pos, float time, Player* player, const GameContext &gameContext)
{
	if (GetCellFromCoord(pos) == CellType::BOMB) return;
	FMOD_RESULT res = m_pFMODSystem->playSound(m_pSoundPlop, 0, false, &m_pSFXChannel);
	SoundManager::ErrorCheck(res);

	m_Bombs.push_back(Bomb());
	m_Bombs[m_Bombs.size() - 1].explosionDistance = explosionDistance;
	m_Bombs[m_Bombs.size() - 1].pos = pos;
	m_Bombs[m_Bombs.size() - 1].timer = time;
	m_Bombs[m_Bombs.size() - 1].pPlayer = player;
	player->DecreaseBombCount();
	SetCell(pos, CellType::BOMB);

	auto pDustCloud = new ParticleEmitterComponent(L"./Resources/Textures/BMAN_Dust_Temp.png", 100);
	pDustCloud->SetVelocity(XMFLOAT3(0, 0, 0));
	pDustCloud->EnableSpawnBasedVelocity(true, 1, 2);
	pDustCloud->SetMinSize(0.2f);
	pDustCloud->SetMaxSize(0.3f);
	pDustCloud->SetMinEnergy(0.3f);
	pDustCloud->SetMaxEnergy(0.5f);
	pDustCloud->SetMinSizeGrow(-1.5f);
	pDustCloud->SetMaxSizeGrow(-1.5);
	pDustCloud->SetMinEmitterRange(0.3f);
	pDustCloud->SetMaxEmitterRange(0.5f);
	pDustCloud->SetColor(XMFLOAT4(0.9f, 0.9f, 0.9f, 1));
	pDustCloud->SetSpawnShape(EmitterSpawnShape::CIRCLE_Y);
	pDustCloud->SetEnabled(false);
	GameObject* pDustEmitter = new GameObject();
	pDustEmitter->AddComponent(pDustCloud);

	auto pFireBall = new ParticleEmitterComponent(L"./Resources/Textures/FireBall.png", 200);
	pFireBall->SetVelocity(XMFLOAT3(0, 2, 0));
	pFireBall->SetMinSize(0.05f);
	pFireBall->SetMaxSize(0.05f);
	pFireBall->SetMinEnergy(0.3f);
	pFireBall->SetMaxEnergy(0.5f);
	pFireBall->SetMinSizeGrow(1);
	pFireBall->SetMaxSizeGrow(1);
	pFireBall->SetMinEmitterRange(0.05f);
	pFireBall->SetMaxEmitterRange(0.05f);
	pFireBall->SetColor(XMFLOAT4(1, 1, 1, 1));
	GameObject* pFireBallEmitter = new GameObject();
	pFireBallEmitter->AddComponent(pFireBall);

	auto pModel = new ModelComponent(L"Resources/Meshes/BMAN_Bomb.ovm");
	pModel->SetMaterial(1);
	auto pBomb = new GameObject();
	pBomb->AddComponent(pModel);
	pBomb->AddChild(pFireBallEmitter);
	pBomb->AddChild(pDustEmitter);
	pDustEmitter->GetTransform()->Translate(XMFLOAT3(0, 0, 50));
	pFireBallEmitter->GetTransform()->Translate(XMFLOAT3(-60, -60, -60));

	m_Bombs[m_Bombs.size() - 1].pBomb = pBomb;
	XMFLOAT2 wPos = GetCellWorldPos(pos);
	pBomb->GetTransform()->Translate(wPos.x, 0, wPos.y);
	pBomb->GetTransform()->Scale({ 0.01f, 0.01f , 0.01f });
	pBomb->GetTransform()->Rotate(XMFLOAT3(90, 180, 0));
	AddChild(pBomb);
	pDustCloud->Burst(gameContext);
}

void PlayingField::DetonateBomb(Bomb & bomb, int &index, const GameContext &gameContext)
{
	FMOD_RESULT res = m_pFMODSystem->playSound(m_pSoundExplosion, 0, false, &m_pSFXChannel);
	SoundManager::ErrorCheck(res);

	{
		auto pExplosion = new ParticleEmitterComponent(L"./Resources/Textures/FireBall.png", 200);
		pExplosion->SetVelocity(XMFLOAT3(0, 0, 0));
		pExplosion->SetSpawnShape(EmitterSpawnShape::CUBE);
		pExplosion->SetMinSize(0.3f);
		pExplosion->SetMaxSize(0.4f);
		pExplosion->SetMinEnergy(0.3f);
		pExplosion->SetMaxEnergy(0.5f);
		pExplosion->SetMinSizeGrow(1);
		pExplosion->SetMaxSizeGrow(1);
		pExplosion->SetMinEmitterRange(0);
		pExplosion->SetMaxEmitterRange(0.3f);
		pExplosion->SetColor(XMFLOAT4(2, 2, 2, 1));
		pExplosion->SetEnabled(false);
		GameObject* pExplosionEmitter = new GameObject();
		pExplosionEmitter->AddComponent(pExplosion);
		XMFLOAT2 cellPos = GetCellWorldPos(bomb.pos);
		pExplosionEmitter->GetTransform()->Translate(XMFLOAT3(cellPos.x, 0, cellPos.y));
		m_Emitters.push_back(std::pair<float, GameObject*>(1, pExplosionEmitter));
		AddChild(pExplosionEmitter);
		pExplosion->Burst(gameContext);
	}


	// **************************** LAMBDA ****************************

	auto checkHits = [&](XMINT2 offset) {
		for (size_t i = 1; i < bomb.explosionDistance + 1; i++) {
			XMINT2 coord;
			coord = bomb.pos;
			coord.x += offset.x * i;
			coord.y += offset.y * i;
			if (coord.x > 0 && coord.x < m_Size.x && coord.y > 0 && coord.y < m_Size.y) {
				if (GetCellFromCoord(coord) == CellType::BLOCK) {
					DestroyWallAtCoord(coord);
					auto pExplosion = new ParticleEmitterComponent(L"./Resources/Textures/FireBall.png", 200);
					pExplosion->SetVelocity(XMFLOAT3(0, 0, 0));
					pExplosion->SetSpawnShape(EmitterSpawnShape::CUBE);
					pExplosion->SetMinSize(0.3f);
					pExplosion->SetMaxSize(0.4f);
					pExplosion->SetMinEnergy(0.3f);
					pExplosion->SetMaxEnergy(0.5f);
					pExplosion->SetMinSizeGrow(1);
					pExplosion->SetMaxSizeGrow(1);
					pExplosion->SetMinEmitterRange(0);
					pExplosion->SetMaxEmitterRange(0.3f);
					pExplosion->SetColor(XMFLOAT4(2, 2, 2, 1));
					pExplosion->SetEnabled(false);
					GameObject* pExplosionEmitter = new GameObject();
					pExplosionEmitter->AddComponent(pExplosion);
					XMFLOAT2 cellPos = GetCellWorldPos(coord);
					pExplosionEmitter->GetTransform()->Translate(XMFLOAT3(cellPos.x, 0, cellPos.y));
					m_Emitters.push_back(std::pair<float, GameObject*>(1, pExplosionEmitter));
					AddChild(pExplosionEmitter);
					pExplosion->Burst(gameContext);
					return;
				}

				if (GetCellFromCoord(coord) == CellType::WALL) {
					return;
				}

				if (GetCellFromCoord(coord) == CellType::BOMB) {
					for (int i = 0; i < m_Bombs.size(); i++) {
						if (m_Bombs[i].pos == coord) {
							m_Bombs[i].timer = 0;
						}
					}
					return;
				}

				auto pExplosion = new ParticleEmitterComponent(L"./Resources/Textures/FireBall.png", 200);
				pExplosion->SetVelocity(XMFLOAT3(0, 0, 0));
				pExplosion->SetSpawnShape(EmitterSpawnShape::CUBE);
				pExplosion->SetMinSize(0.3f);
				pExplosion->SetMaxSize(0.4f);
				pExplosion->SetMinEnergy(0.3f);
				pExplosion->SetMaxEnergy(0.5f);
				pExplosion->SetMinSizeGrow(1);
				pExplosion->SetMaxSizeGrow(1);
				pExplosion->SetMinEmitterRange(0);
				pExplosion->SetMaxEmitterRange(0.3f);
				pExplosion->SetColor(XMFLOAT4(2, 2, 2, 1));
				pExplosion->SetEnabled(false);
				GameObject* pExplosionEmitter = new GameObject();
				pExplosionEmitter->AddComponent(pExplosion);
				XMFLOAT2 cellPos = GetCellWorldPos(coord);
				pExplosionEmitter->GetTransform()->Translate(XMFLOAT3(cellPos.x, 0, cellPos.y));
				m_Emitters.push_back(std::pair<float, GameObject*>(1, pExplosionEmitter));
				AddChild(pExplosionEmitter);
				pExplosion->Burst(gameContext);
			} else {
				return;
			}
		}
		return;
	};

	// **************************** LAMBDA END ****************************

	checkHits({ -1, 0 });
	checkHits({ 1, 0 });
	checkHits({ 0, 1 });
	checkHits({ 0, -1 });

	SetCell(bomb.pos, CellType::EMPTY);
	bomb.pPlayer->IncreaseBombCount();
	RemoveChild(bomb.pBomb);
	delete bomb.pBomb;
	m_Bombs.erase(m_Bombs.begin() + index);
	--index;
}

void PlayingField::SetCell(const XMINT2 & coord, const CellType & type)
{
	m_Cells[coord.y * m_Size.x + coord.x] = type;
}

void PlayingField::DestroyWallAtCoord(const XMINT2 &coord)
{
	int index = coord.y * m_Size.x + coord.x;
	auto it = m_pBlocks.find(index);

	if (it != m_pBlocks.end()) {
		m_Cells[index] = CellType::EMPTY;
		RemoveChild(it->second);
		delete it->second;
		m_pBlocks.erase(it);
	}
}

void PlayingField::UpdateBombs(const GameContext &gameContext)
{
	for (int i = 0; i < m_Bombs.size(); i++) {
		m_Bombs[i].timer -= gameContext.pGameTime->GetElapsed();
		float d = m_Bombs[i].timer * 2;
		d = d > 1 ? 1 : d;
		float m = d * (sin(m_Bombs[i].timer * 10) * 0.001f) + (1 - d) * 0.004f;
		m_Bombs[i].pBomb->GetTransform()->Scale({ 0.01f + m, 0.01f + m, 0.01f + m });
		if (m_Bombs[i].timer <= 0) {
			DetonateBomb(m_Bombs[i], i, gameContext);
		}
	}
}

void PlayingField::UpdateEmitters(const GameContext &gameContext)
{
	for (int i = 0; i < m_Emitters.size(); i++) {
		m_Emitters[i].first -= gameContext.pGameTime->GetElapsed();
		if (m_Emitters[i].first < 0) {
			RemoveChild(m_Emitters[i].second);
			delete m_Emitters[i].second;
			m_Emitters.erase(m_Emitters.begin() + i);
			--i;
		} else if (m_Emitters[i].first > 0.5f) {
			XMFLOAT3 worldPos = m_Emitters[i].second->GetTransform()->GetWorldPosition();
			XMFLOAT2 worldCoord = { worldPos.x, worldPos.z };
			XMINT2 coord = GetCellCoord(worldCoord);

			XMFLOAT3 player1Pos = m_pPlayer1->GetTransform()->GetPosition();
			XMFLOAT2 player1Pos2D;
			player1Pos2D.x = player1Pos.x;
			player1Pos2D.y = player1Pos.z;

			XMINT2 player1Coord = GetCellCoord(player1Pos2D);

			if (coord == player1Coord) {
				m_pPlayer1->Kill(gameContext);
			}

			XMFLOAT3 player2Pos = m_pPlayer2->GetTransform()->GetPosition();
			XMFLOAT2 player2Pos2D;
			player2Pos2D.x = player2Pos.x;
			player2Pos2D.y = player2Pos.z;

			XMINT2 player2Coord = GetCellCoord(player2Pos2D);

			if (coord == player2Coord) {
				m_pPlayer2->Kill(gameContext);
			}
		}
	}
}

void PlayingField::UpdatePickups(const GameContext & gameContext)
{
	for (size_t i = 0; i < m_Pickups.size(); i++) {
		XMFLOAT3 pos = m_Pickups[i].second->GetTransform()->GetPosition();
		XMFLOAT2 coordPos = { pos.x, pos.z };
		XMINT2 coord = GetCellCoord(coordPos);

		XMFLOAT3 player1Pos = m_pPlayer1->GetTransform()->GetPosition();
		XMFLOAT2 player1Pos2D;
		player1Pos2D.x = player1Pos.x;
		player1Pos2D.y = player1Pos.z;

		XMINT2 player1Coord = GetCellCoord(player1Pos2D);

		if (coord == player1Coord) {
			FMOD_RESULT res = m_pFMODSystem->playSound(m_pSoundPickup, 0, false, &m_pSFXChannel);
			SoundManager::ErrorCheck(res);

			switch (m_Pickups[i].first) {
			case PickupType::BOMBAMOUNT:
				m_pPlayer1->IncreaseBombCount();
				break;
			case PickupType::BOMBPOWER:
				m_pPlayer1->IncreaseBombPower();
				break;
			case PickupType::SPEED:
				m_pPlayer1->IncreaseSpeed();
				break;
			default:
				break;
			}

			RemoveChild(m_Pickups[i].second);
			delete m_Pickups[i].second;
			m_Pickups.erase(m_Pickups.begin() + i);
			--i;
		} else {
			XMFLOAT3 player2Pos = m_pPlayer2->GetTransform()->GetPosition();
			XMFLOAT2 player2Pos2D;
			player2Pos2D.x = player2Pos.x;
			player2Pos2D.y = player2Pos.z;

			XMINT2 player2Coord = GetCellCoord(player2Pos2D);

			if (coord == player2Coord) {
				FMOD_RESULT res = m_pFMODSystem->playSound(m_pSoundPickup, 0, false, &m_pSFXChannel);
				SoundManager::ErrorCheck(res);

				switch (m_Pickups[i].first) {
				case PickupType::BOMBAMOUNT:
					m_pPlayer2->IncreaseBombCount();
					break;
				case PickupType::BOMBPOWER:
					m_pPlayer2->IncreaseBombPower();
					break;
				case PickupType::SPEED:
					m_pPlayer2->IncreaseSpeed();
					break;
				default:
					break;
				}

				RemoveChild(m_Pickups[i].second);
				delete m_Pickups[i].second;
				m_Pickups.erase(m_Pickups.begin() + i);
				--i;
			}
		}
	}
}

PlayingField::CellType PlayingField::GetCellFromWorldPos(const XMFLOAT2 &worldPos) const
{
	XMINT2 result = GetCellCoord(worldPos);
	return GetCellFromCoord(result);
}