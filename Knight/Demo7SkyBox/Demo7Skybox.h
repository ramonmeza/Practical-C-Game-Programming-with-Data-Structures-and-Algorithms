#pragma once

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "Knight.h"   //Engine includes

#include "SkyboxComponent.h"

#include <vector>

class Demo6Skybox : public Knight
{
public:
	void Start() override;
	void EndGame() override;

	Demo6Skybox();

	PerspectiveCamera* pMainCamera;
	SceneActor* Actor = NULL;
	
	SkyboxComponent* pSkyBox = NULL;

protected:

	void Update(float ElapsedSeconds) override;
	void DrawFrame() override;
	void DrawGUI() override;
};

