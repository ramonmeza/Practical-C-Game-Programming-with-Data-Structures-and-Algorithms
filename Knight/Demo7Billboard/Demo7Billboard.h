#pragma once

#include "Knight.h"   //Engine includes
#include "BillboardComponent.h"

class Demo7Billboard : public Knight
{
public:
	void Start() override;
	void EndGame() override;

	FlyThroughCamera* mainCamera = NULL;
	SceneActor* player = NULL;
	vector<SceneActor*> imposters;

	Texture2D billboardImage = { 0 };

protected:

	void Update(float ElapsedSeconds) override;
	void DrawFrame() override;
	void DrawGUI() override;
};
