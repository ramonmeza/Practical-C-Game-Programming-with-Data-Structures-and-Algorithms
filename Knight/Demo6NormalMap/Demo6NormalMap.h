#pragma once

#include "Knight.h"   //Engine includes

class Demo6NormalMap : public Knight
{
public:
	void Start() override;
	void EndGame() override;

	Demo6NormalMap();

	FlyThroughCamera* pMainCamera = NULL;
	SceneActor* Actor = NULL;

	Model model = { 0 };
	Model model2 = { 0 };
	Vector3 modelpos = { 0,0,0 };
	Vector3 modelpos2 = { -2,0,0 };
	Texture2D diffuse = { 0 };
	Texture2D normalMap = { 0 };

	Shader shader = { 0 };

	Vector3 lightPos = { 0, 2, 0.0f };
	int lightPosLoc = -1;
	int lightPowerLoc = -1;
	int lightColorLoc = -1;
	Vector3 lightColor = { 1,1,1 };
	float lightPower = 40.0f;

protected:

	void Update(float ElapsedSeconds) override;
	void DrawFrame() override;
	void DrawGUI() override;
};


