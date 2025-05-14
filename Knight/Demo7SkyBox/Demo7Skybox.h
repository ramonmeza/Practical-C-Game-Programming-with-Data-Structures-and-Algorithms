#pragma once

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "Knight.h"   

#include "SkyboxComponent.h"

#include <vector>

class Demo7Skybox : public Knight
{
public:
	void Start() override;
	void EndGame() override;

	Demo7Skybox();

	PerspectiveCamera* pMainCamera = nullptr;
	SceneActor* Actor = nullptr;	
	SkyboxComponent* pSkyBox = nullptr;

protected:

	void Update(float ElapsedSeconds) override;
	void DrawFrame() override;
	void DrawGUI() override;
};

