#include "Demo53PV.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <cmath>

int main(int argc, char* argv[])
{
	Demo53PV* KnightDemo53PV = new Demo53PV();

	KnightDemo53PV->Start();
	KnightDemo53PV->GameLoop();

	delete KnightDemo53PV;
	return 0;
}

Demo53PV::Demo53PV()
{
}

void Demo53PV::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	ShowFPS = true;

	//initialize global UI attributes
	GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

	//initialize global UI attributes
	GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

	ChaseCamera = _Scene->CreateSceneObject<FollowUpCamera>("Chase Camera");

	//Place player
	Actor = _Scene->CreateSceneObject<SceneActor>("Player");
	Actor->Scale = Vector3{ 0.3f, 0.3f, 0.3f };
	Actor->Position = Vector3{ 0.f,0.5f,0.f };
	Actor->Rotation = Vector3{ 0,0,0 };
	ModelComponent* animPlayerComponent = Actor->CreateAndAddComponent<ModelComponent>();
	animPlayerComponent->Load3DModel("../../resources/models/gltf/robot.glb");
	animPlayerComponent->SetAnimation(6);
	Actor->AddComponent(animPlayerComponent);

	ChaseCamera->SetUp(Actor, 45.0f, 5.0f, CAMERA_PERSPECTIVE);
}

void Demo53PV::EndGame()
{
	__super::EndGame();
}

void Demo53PV::Update(float ElapsedSeconds)
{
	if (IsKeyDown(KEY_W)) {
		// Move player forward based on their rotation
		Actor->Position.x += sin(DegreesToRadians(Actor->Rotation.y)) * 0.1f;
		Actor->Position.z += cos(DegreesToRadians(Actor->Rotation.y)) * 0.1f;
	}
	if (IsKeyDown(KEY_S)) {
		// Move player backward based on their rotation
		Actor->Position.x -= sin(DegreesToRadians(Actor->Rotation.y)) * 0.1f;
		Actor->Position.z -= cos(DegreesToRadians(Actor->Rotation.y)) * 0.1f;
	}
	if (IsKeyDown(KEY_A)) {
		Actor->Rotation.y += 1;  // Rotate left
	}
	if (IsKeyDown(KEY_D)) {
		Actor->Rotation.y -= 1;  // Rotate right
	}

	__super::Update(ElapsedSeconds);
}

void Demo53PV::DrawFrame()
{
	__super::DrawFrame();

	// Draw grid for better spatial reference
	DrawGrid(10, 1.0f);

	Vector3 forward = {
		Actor->Position.x + sin(DegreesToRadians(Actor->Rotation.y)),
		Actor->Position.y,
		Actor->Position.z + cos(DegreesToRadians(Actor->Rotation.y))
	};
	DrawLine3D(Actor->Position, forward, GREEN);

}

void Demo53PV::DrawGUI()
{
	__super::DrawGUI();

	DrawText("Use WASD to move player", 10, 40, 20, DARKGRAY);
	DrawText("Hold right mouse button to rotate camera", 10, 70, 20, DARKGRAY);
	DrawText("Use mouse wheel to adjust camera distance", 10, 100, 20, DARKGRAY);
}

