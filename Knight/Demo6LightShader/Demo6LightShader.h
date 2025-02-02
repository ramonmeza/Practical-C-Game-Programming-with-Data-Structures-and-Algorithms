#pragma once

#include "Knight.h"   //Engine includes
#include "SceneLight.h"

class Demo6LightShader : public Knight
{
public:
	void Start() override;
	void EndGame() override;

	Demo6LightShader();

	FlyThroughCamera* pMainCamera = NULL;
	SceneActor* player = NULL;

	SceneLight* pLight = NULL;

	// Define the base color of the cube
	Vector4 baseColor = { 0.3f, 0.6f, 0.9f, 1.0f }; // Light blue color

protected:

	void Update(float ElapsedSeconds) override;
	void DrawFrame() override;
	void DrawGUI() override;
};

