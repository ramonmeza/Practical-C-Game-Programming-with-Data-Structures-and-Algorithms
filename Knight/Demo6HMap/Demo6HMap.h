#pragma once

#include "Knight.h"   //Engine includes

#include "HMapTerrainModelComponent.h"

class Demo6HMap : public Knight
{
public:
	void Start() override;
	void EndGame() override;

	Demo6HMap();

	PerspectiveCamera* pMainCamera = NULL;
	SceneActor* pTerrain = NULL;

	Image image = { 0 };
	Texture2D texture = { 0 };

protected:

	void Update(float ElapsedSeconds) override;
	void DrawFrame() override;
	void DrawGUI() override;
};


