#include "Demo6LightShader.h"

#include "raymath.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <cmath>

int main(int argc, char* argv[])
{
	Demo6LightShader* KnightDemo6Billboard = new Demo6LightShader();

	KnightDemo6Billboard->Start();
	KnightDemo6Billboard->GameLoop();

	delete KnightDemo6Billboard;
	return 0;
}

Demo6LightShader::Demo6LightShader()
{
}

void Demo6LightShader::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	ShowFPS = true;

	//initialize global UI attributes
	GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

	// Create lights
	pMainCamera = _Scene->CreateSceneObject<FlyThroughCamera>("Main Camera");
	pMainCamera->SetUp(Vector3{ 0, 0.5, 0 }, 20.0f, 0, 40.0f, 45.0f, CAMERA_PERSPECTIVE);

	pLight = _Scene->CreateSceneObject<SceneLight>("Light");
	pLight->Scale = Vector3{ 1,1, 1 };
	pLight->Position = Vector3{ 3.0f, 5.0f, -3.0f };
	pLight->Rotation = Vector3{ 0,0,0 };
	pLight->Create();

	//Place player
	player = _Scene->CreateSceneObject<SceneActor>("player");
	player->Scale = Vector3{ 1,1,1 };
	player->Position = Vector3{ 0.f,0.5f,0.f };
	player->Rotation = Vector3{ 0,0,0 };
	ModelComponent* animPlayerComponent = player->CreateAndAddComponent<ModelComponent>();
	animPlayerComponent->Load3DModel("../../resources/models/gltf/robot.glb");
	animPlayerComponent->SetShader(&pLight->shader, -1);
	animPlayerComponent->SetAnimation(6);
	player->AddComponent(animPlayerComponent);

	
}

void Demo6LightShader::EndGame()
{
	__super::EndGame();
}

void Demo6LightShader::Update(float ElapsedSeconds)
{
	if (IsKeyDown(KEY_W)) {
		// Move player forward based on their rotation
		player->Position.x += sin(DegreesToRadians(player->Rotation.y)) * 0.1f;
		player->Position.z += cos(DegreesToRadians(player->Rotation.y)) * 0.1f;
	}
	if (IsKeyDown(KEY_S)) {
		// Move player backward based on their rotation
		player->Position.x -= sin(DegreesToRadians(player->Rotation.y)) * 0.1f;
		player->Position.z -= cos(DegreesToRadians(player->Rotation.y)) * 0.1f;
	}
	if (IsKeyDown(KEY_A)) {
		player->Rotation.y += 1;  // Rotate left
	}
	if (IsKeyDown(KEY_D)) {
		player->Rotation.y -= 1;  // Rotate right
	}

	// Update light position for animation (simple circular motion)
	pLight->Position.x = 4.0f * cos(GetTime());
	pLight->Position.z = 4.0f * sin(GetTime());
	pLight->lightDirection = Vector3Normalize(Vector3Subtract(Vector3{ 0,0,0 }, pLight->Position));
	__super::Update(ElapsedSeconds);
}

void Demo6LightShader::DrawFrame()
{
	__super::DrawFrame();

	DrawSphereWires(pLight->Position, 0.2f, 8, 8, WHITE);

	// Draw grid for better spatial reference
	DrawGrid(10, 1.0f);
}

void Demo6LightShader::DrawGUI()
{
	__super::DrawGUI();

}

