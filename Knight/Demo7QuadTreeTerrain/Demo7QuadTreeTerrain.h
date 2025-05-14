#pragma once

#include "Knight.h"   //Engine includes
#include "QuadTreeTerrainModelComponent.h" // QuadTreeTerrainModelComponent class

class Demo7QuadTreeTerrain : public Knight
{
public:
	void Start() override;
	void EndGame() override;

	Demo7QuadTreeTerrain();

	FlyThroughCamera* pMainCamera = nullptr;
	SceneActor* pTerrain = NULL;
	QuadTreeTerrainModelComponent* pQuadTreeTerrain = nullptr;


protected:

	void Update(float ElapsedSeconds) override;
	void DrawFrame() override;
	void DrawGUI() override;   
};


