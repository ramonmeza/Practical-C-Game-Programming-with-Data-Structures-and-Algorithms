#pragma once

#include "Knight.h"   //Engine includes

#define MAX_COLUMNS 20

class Demo5FPC : public Knight
{
	public:
		PerspectiveCamera* FPSCamera = nullptr;

		SceneActor* pFloor = nullptr;
		SceneActor* pWall1 = nullptr;
		SceneActor* pWall2 = nullptr;
		SceneActor* pWall3 = nullptr;
		SceneActor* pWall4 = nullptr;
		SceneActor* pProp1 = nullptr;
		SceneActor* pProp2 = nullptr;

		// Generates some random columns
		float heights[MAX_COLUMNS] = { 0 };
		Vector3 positions[MAX_COLUMNS] = { 0 };
		Color colors[MAX_COLUMNS] = { 0 };

		void Start() override;

	protected:

		void DrawGUI() override;
		void OnCreateDefaultResources() override;
};


