#pragma once
#include "Scenegraph/GameScene.h"

class TextureData;
class SpriteComponent;
class ControlsScreen : public GameScene
{
public:
	ControlsScreen(void);
	virtual ~ControlsScreen(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ControlsScreen(const ControlsScreen &obj);
	ControlsScreen& operator=(const ControlsScreen& obj);
};

