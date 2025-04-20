#include "Demo5MultiCams.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <cmath>

int main(int argc, char* argv[])
{
	Demo5MultiCams* KnightDemo5MultiCams = new Demo5MultiCams();

	KnightDemo5MultiCams->Start();
	KnightDemo5MultiCams->GameLoop();

	delete KnightDemo5MultiCams;
	return 0;
}

Demo5MultiCams::Demo5MultiCams()
{
}

void Demo5MultiCams::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	ShowFPS = true;

	//initialize global UI attributes
	GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

	// Create camera for both render targets
	pChaseCamera = new FollowUpCamera(_Scene, "Chase Camera", false);
	pTopDownCamera = new TopDownCamera(_Scene, "Map Camera", false);

	//Place player
	Actor = _Scene->CreateSceneObject<SceneActor>("Player");
	Actor->Scale = Vector3{ 0.3f, 0.3f, 0.3f };
	Actor->Position = Vector3{ 0.f,0.1f,0.f };
	Actor->Rotation = Vector3{ 0,0,0 };
	ModelComponent* animPlayerComponent = Actor->CreateAndAddComponent<ModelComponent>();
	animPlayerComponent->Load3DModel("../../resources/models/gltf/robot.glb");
	animPlayerComponent->SetAnimation(6);
	Actor->AddComponent(animPlayerComponent);

	// Setup player 1 camera and screen	
	pChaseCamera->SetUp(Actor, 45.0f, 5.0f, CAMERA_PERSPECTIVE);
	pChaseCamera->processMouseInput = false;

	pTopDownCamera->SetUp(Vector3{ -3.0f, 3.0f, 0 }, Actor->Position, 45.0f, CAMERA_PERSPECTIVE);

	ChaseCamRT = LoadRenderTexture(SCREEN_WIDTH / 2, SCREEN_HEIGHT);
	TopDownCamRT = LoadRenderTexture(SCREEN_WIDTH / 2, SCREEN_HEIGHT);

	splitScreenRect = { 0.0f, 0.0f, (float)ChaseCamRT.texture.width, (float)-ChaseCamRT.texture.height };
}

void Demo5MultiCams::EndGame()
{
	__super::EndGame();
}

void Demo5MultiCams::Update(float ElapsedSeconds)
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

	pChaseCamera->Update(ElapsedSeconds);

	pTopDownCamera->SetLookAtPosition(Actor->Position);
	pTopDownCamera->Update(ElapsedSeconds);

	__super::Update(ElapsedSeconds);
}

void Demo5MultiCams::DrawOffscreen()
{
	// Draw Player1 view to the render texture
	BeginTextureMode(ChaseCamRT);
	ClearBackground(DARKBLUE);

	BeginMode3D(*pChaseCamera->GetCamera3D());
	DrawGameWorld(pChaseCamera);
	EndMode3D();

	DrawRectangle(0, 0, GetScreenWidth() / 2, 40, Fade(RAYWHITE, 0.8f));
	DrawText("The 3rd person camera view", 10, 50, 20, MAROON);

	EndTextureMode();

	// Draw Player2 view to the render texture
	BeginTextureMode(TopDownCamRT);
	ClearBackground(DARKPURPLE);

	BeginMode3D(*pTopDownCamera->GetCamera3D());
	DrawGameWorld(pTopDownCamera);
	EndMode3D();

	DrawRectangle(0, 0, GetScreenWidth() / 2, 40, Fade(RAYWHITE, 0.8f));
	DrawText("The top down camera view", 10, 50, 20, DARKBLUE);

	EndTextureMode();
}

void Demo5MultiCams::DrawFrame()
{
}

void Demo5MultiCams::DrawGUI()
{
	__super::DrawGUI();

	DrawTextureRec(ChaseCamRT.texture, splitScreenRect, Vector2{ 0, 0 }, WHITE);
	DrawTextureRec(TopDownCamRT.texture, splitScreenRect, Vector2{ SCREEN_WIDTH / 2.0f, 0 }, WHITE);

	DrawRectangle(GetScreenWidth() / 2 - 2, 0, 4, GetScreenHeight(), LIGHTGRAY);
}

void Demo5MultiCams::DrawGameWorld(SceneCamera *pCam)
{
	_Scene->DrawFrame();
	DrawGrid(10, 1);
}
