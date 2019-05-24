#pragma once
#include "../Scenegraph/GameObject.h"
class SkyBoxPrefab : public GameObject
{
public:
	SkyBoxPrefab(const wstring &assetFile);
	~SkyBoxPrefab();
protected:
	virtual void Initialize(const GameContext& gameContext);
	wstring m_CubeMapFile;
private:
	SkyBoxPrefab(const SkyBoxPrefab &obj) = delete;
	SkyBoxPrefab& operator=(const SkyBoxPrefab &obj) = delete;
};