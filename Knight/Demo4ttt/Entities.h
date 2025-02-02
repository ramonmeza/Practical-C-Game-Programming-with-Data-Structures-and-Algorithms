#pragma once

#include "Knight.h"

class Entity
{
public:
	// method to render the entire level
	virtual void Create(Scene* pScene, Entity* pParent) = 0;
	virtual void Update(float elaspedTime);
	virtual void DrawGUI();
	SceneActor* Actor;
	Entity();
};


