#pragma once
#include "..\Scenegraph\GameObject.h"

class CubePrefab: public GameObject
{
public:
	CubePrefab(float width = 1.f , float height = 1.f, float depth = 1.f, XMFLOAT4 color = static_cast<XMFLOAT4>(Colors::Red));
	~CubePrefab(void);

protected:

	void Initialize(const GameContext& gameContext) override;

private:

	float m_Width, m_Height, m_Depth;
	XMFLOAT4 m_Color;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	CubePrefab(const CubePrefab& yRef) = delete;									
	CubePrefab& operator=(const CubePrefab& yRef) = delete;
};
