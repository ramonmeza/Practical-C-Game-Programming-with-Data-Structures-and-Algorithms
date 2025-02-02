#pragma once

#include "Knight.h"   //Engine includes
#include "WaypointsCamera.h"

#define MAX_COLUMNS 20

class Demo5TrkCam : public Knight
{
public:
	void Start() override;
	void EndGame() override;

	Demo5TrkCam();

	WaypointsCamera* OnRailCamera = NULL;
	SceneActor* Actor = NULL;

	// Moving cube parameters
	Vector3 cubePosition = { -10.0f, 1.0f, 0.0f }; // Initial position of the moving cube
	Vector3 cubeDirection = { 0.1f, 0.0f, 0.0f };  // Direction and speed of the cube


protected:

	void Update(float ElapsedSeconds) override;
	void DrawFrame() override;
	void DrawGUI() override;
};

