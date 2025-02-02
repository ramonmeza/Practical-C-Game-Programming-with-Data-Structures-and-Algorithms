#pragma once

#include "Defs.h" 
#include "Scene.h"
#include "SceneObject.h"
#include "SceneActor.h"
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

	bool ShowFPS = false;	

protected:
	virtual void Update(float ElapsedSeconds);
	virtual void DrawOffscreen();
	virtual void DrawFrame();
	virtual void DrawGUI();

protected:
	Scene* _Scene;
	Font _Font;
	bool _shouldExitGameLoop;

	SceneRenderPass* pDefaultRenderer = nullptr;
};

