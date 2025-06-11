#pragma once

#include "Defs.h" 
#include "Scene.h"
#include "SceneObject.h"
#include "SceneActor.h"
#include "SceneCamera.h"
#include "PerspectiveCamera.h"
#include "OrthogonalCamera.h"
#include "FlyThroughCamera.h"
#include "Component.h"
#include "ModelComponent.h"
#include "CubeComponent.h"
#include "SphereComponent.h"
#include "PlaneComponent.h"
#include "CylinderComponent.h"
#include "ConeComponent.h"
#include "SceneRenderPass.h"
#include "ForwardRenderPass.h"
#include "KnightUtils.h"

struct KnightConfig
{
	bool ShowFPS = false;
	bool ShowDebugInfo = false;
};

class Knight
{
public:
	Knight();
	~Knight();

public:
	virtual void Start();
	void GameLoop();
	virtual void EndGame();
	void ExitGameLoop();

	void DrawText(const char* text, int x, int y, int size, const Color& color = Color{ 255,255,255,255 });
	void SaveScreenshot(const char* fileName);

	//bool ShowFPS = false;
	KnightConfig Config;

	static Knight* Instance;

protected:
	virtual void Update(float ElapsedSeconds);
	virtual void DrawOffscreen();
	virtual void DrawFrame();
	virtual void DrawGUI();

	void DrawFPS(int x, int y);


protected:
	Scene* _Scene;
	Font _Font;
	bool _shouldExitGameLoop;

	virtual void OnCreateDefaultResources();
	virtual void OnReleaseDefaultResources();
	virtual void OnConfigKnightApp();
};

