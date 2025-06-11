#pragma once

#include "Knight.h"   //Engine includes
#include "SceneLight.h"

class Demo6LightShader : public Knight
{
public:
	void Start() override;

	Demo6LightShader();

	FlyThroughCamera* pMainCamera = nullptr;
	SceneActor* player = nullptr;

	SceneLight* pLight = nullptr;

protected:

	void Update(float ElapsedSeconds) override;
	void DrawFrame() override;
	void DrawGUI() override;
	void OnCreateDefaultResources() override;
};

