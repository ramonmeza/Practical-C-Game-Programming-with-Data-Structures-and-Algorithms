#pragma once

#include "raylib.h"
#include "Knight.h"   //Engine includes

#include "TopDownCamera.h"

class Demo6GameWorld : public Knight
{
public:
	void Start() override;
	void EndGame() override;

	Demo6GameWorld();

	TopDownCamera* pMainCamera = NULL;
	SceneActor* Actor = NULL;
	Vector3 LookAt = { 0 };

	Image image = { 0 };
	Texture2D texture = { 0 };

	Mesh mesh = { 0 }; // Generate heightmap mesh (RAM and VRAM)
	Model model = { 0 };                  // Load model from generated mesh
	Vector3 mapPosition = { 0 };

protected:

	void Update(float ElapsedSeconds) override;
	void DrawFrame() override;
	void DrawGUI() override;
};

