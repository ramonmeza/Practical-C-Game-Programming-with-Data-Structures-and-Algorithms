#pragma once

#include "Knight.h"   //Engine includes
#include "FollowUpCamera.h"

#define MAX_COLUMNS 20

class Demo53PV : public Knight
{
public:
	void Start() override;
	void EndGame() override;

	Demo53PV();
	
	FollowUpCamera* ChaseCamera;
	SceneActor* Actor = NULL;

protected:

	void Update(float ElapsedSeconds) override;
	void DrawFrame() override;
	void DrawGUI() override;
};


