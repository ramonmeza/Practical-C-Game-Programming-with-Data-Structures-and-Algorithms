#pragma once

#include "Knight.h"   //Engine includes
#include "FollowUpCamera.h"
#include "TopDownCamera.h"

class Demo5MultiCams : public Knight
{
public:
	void Start() override;
	void EndGame() override;

	Demo5MultiCams();

	SceneActor* Actor = NULL;

	//Camera3D cameraPlayer1 = { 0 };
	//Camera3D cameraPlayer2 = { 0 };
	FollowUpCamera* pChaseCamera = nullptr;
	TopDownCamera* pTopDownCamera = nullptr;

	Rectangle splitScreenRect = { 0 };
	RenderTexture ChaseCamRT = { 0 };
	RenderTexture TopDownCamRT = { 0 };

protected:

	void Update(float ElapsedSeconds) override;
	void DrawOffscreen() override;
	void DrawFrame() override;
	void DrawGUI() override;

	void DrawGameWorld(SceneCamera* pCam);
};


