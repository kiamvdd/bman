#pragma once
#include "Scenegraph/GameScene.h"
class Player;
class PlayingField;
class FixedCamera;
class SpriteComponent;
class SpriteFont;
class BombermanLevel : public GameScene
{
public:
	BombermanLevel(void);
	virtual ~BombermanLevel(void);	
protected:

	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
private:
	void Reset(const GameContext& gameContext, bool full = false);
	void UpdateUI(const GameContext& gameContext);
	void ClearPPEffects(const GameContext& gameContext);
	void NextPPEffect(const GameContext& gameContext);
	void PreviousPPEffect(const GameContext& gameContext);
	void NextPPPreset(const GameContext& gameContext);
	void PreviousPPPreset(const GameContext& gameContext);
private:
	Player * m_pPlayer1 = nullptr;
	Player* m_pPlayer2 = nullptr;
	PlayingField* m_pField = nullptr;
	FixedCamera* m_Camera = nullptr;
	float m_RespawnTimer = 5;

	float m_MenuOffset = -720;
	bool m_Paused = false;
	GameObject* m_MenuBackground;
	SpriteComponent* m_MainMenuSprite;
	GameObject* m_MainMenuButton;
	bool m_MenuSelected = false;
	SpriteComponent* m_RestartSprite;
	GameObject* m_RestartButton;
	bool m_RestartSelected = false;
	SpriteComponent* m_ExitSprite;
	GameObject* m_ExitButton;
	bool m_ExitSelected = false;
	SpriteComponent* m_ResultScreen;
	SpriteComponent* m_VictoryText;

	int m_Preset = 0;
	int m_PPEffect = 0;
	int m_P1Score = 0;
	int m_P2Score = 0;
	bool m_RoundEnded = false;
	bool m_GameEnded = false;
	std::vector<bool> m_PPEffects;
	SpriteFont* m_pFont;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	BombermanLevel(const BombermanLevel &obj);
	BombermanLevel& operator=(const BombermanLevel& obj);
};