#pragma once
#include "Scenegraph/GameScene.h"

class TextureData;
class SpriteComponent;
class MainMenu : public GameScene
{
public:
	MainMenu(void);
	virtual ~MainMenu(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	float m_FpsInterval;
	TextureData* m_pTexture;

	SpriteComponent* m_StartButton;
	bool m_StartSelected = false;
	SpriteComponent* m_ExitButton;
	bool m_ExitSelected = false;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	MainMenu(const MainMenu &obj);
	MainMenu& operator=(const MainMenu& obj);
};

