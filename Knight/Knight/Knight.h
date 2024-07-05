#pragma once

#include "Defs.h" 
#include "Scene.h"
#include "SceneObject.h"
#include "SceneActor.h"
#include "PerspectiveCamera.h"
#include "Component.h"
#include "ModelComponent.h"
#include "CubeComponent.h"
#include "SphereComponent.h"
#include "PlaneComponent.h"
#include "CylinderComponent.h"
#include "ConeComponent.h"

class Knight
{
public:
	Knight();
	~Knight();

public:
	virtual void Start();
	void GameLoop();
	virtual void EndGame();

	bool ShowFPS = false;

protected:
	virtual void Update(float ElapsedSeconds);
	virtual void DrawFrame();

protected:
	Scene* _Scene;
	Font _Font;

};

