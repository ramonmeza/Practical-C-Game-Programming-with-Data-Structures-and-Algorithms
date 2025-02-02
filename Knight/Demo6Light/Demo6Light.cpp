#include "Demo6Light.h"

#include "raymath.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

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
	Demo6Light* KnightDemo6Billboard = new Demo6Light();

	KnightDemo6Billboard->Start();
	KnightDemo6Billboard->GameLoop();

	delete KnightDemo6Billboard;
	return 0;
}

void Demo6Light::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	ShowFPS = true;

	//initialize global UI attributes
	GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

	SetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)

	shader = LoadShader(TextFormat("../../resources/shaders/glsl%i/lighting.vs", GLSL_VERSION),
		TextFormat("../../resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));
	shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
	ambientLoc = GetShaderLocation(shader, "ambient");
	const float ac[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	SetShaderValue(shader, ambientLoc, ac , SHADER_UNIFORM_VEC4);

	// Create lights
	
	lights[0] = CreateLight(LIGHT_POINT, Vector3 { -2, 1, -2 }, Vector3Zero(), YELLOW, shader);
	lights[1] = CreateLight(LIGHT_POINT, Vector3 { 2, 1, 2 }, Vector3Zero(), RED, shader);
	lights[2] = CreateLight(LIGHT_POINT, Vector3 { -2, 1, 2 }, Vector3Zero(), GREEN, shader);
	lights[3] = CreateLight(LIGHT_POINT, Vector3 { 2, 1, -2 }, Vector3Zero(), BLUE, shader);

	pMainCamera = _Scene->CreateSceneObject<FlyThroughCamera>("Main Camera");
	pMainCamera->SetUp(Vector3{0,0,0}, 6.0f, 0, 30.0f, 45.0f, CAMERA_PERSPECTIVE);

	//Place player
	actor = _Scene->CreateSceneObject<SceneActor>("Player");
	actor->Scale = Vector3{ 0.3f, 0.3f, 0.3f };
	actor->Position = Vector3{ 0.f,0.5f,0.f };
	actor->Rotation = Vector3{ 0,0,0 };
	ModelComponent* animPlayerComponent = actor->CreateAndAddComponent<ModelComponent>();
	animPlayerComponent->Load3DModel("../../resources/models/gltf/robot.glb");
	animPlayerComponent->SetShader(&shader, -1);
	animPlayerComponent->SetAnimation(6);
	actor->AddComponent(animPlayerComponent);

}

void Demo6Light::EndGame()
{
	__super::EndGame();
}

void Demo6Light::Update(float ElapsedSeconds)
{
	if (IsKeyDown(KEY_W)) {
		// Move player forward based on their rotation
		actor->Position.x += sin(DegreesToRadians(actor->Rotation.y)) * 0.1f;
		actor->Position.z += cos(DegreesToRadians(actor->Rotation.y)) * 0.1f;
	}
	if (IsKeyDown(KEY_S)) {
		// Move player backward based on their rotation
		actor->Position.x -= sin(DegreesToRadians(actor->Rotation.y)) * 0.1f;
		actor->Position.z -= cos(DegreesToRadians(actor->Rotation.y)) * 0.1f;
	}
	if (IsKeyDown(KEY_A)) {
		actor->Rotation.y += 1;  // Rotate left
	}
	if (IsKeyDown(KEY_D)) {
		actor->Rotation.y -= 1;  // Rotate right
	}

	float cameraPos[3] = { pMainCamera->GetPosition().x, pMainCamera->GetPosition().y, pMainCamera->GetPosition().z};
	SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

	// Check key inputs to enable/disable lights
	if (IsKeyPressed(KEY_Y)) { lights[0].enabled = !lights[0].enabled; }
	if (IsKeyPressed(KEY_R)) { lights[1].enabled = !lights[1].enabled; }
	if (IsKeyPressed(KEY_G)) { lights[2].enabled = !lights[2].enabled; }
	if (IsKeyPressed(KEY_B)) { lights[3].enabled = !lights[3].enabled; }

	for (int i = 0; i < MAX_LIGHTS; i++) UpdateLightValues(shader, lights[i]);

	__super::Update(ElapsedSeconds);
}

void Demo6Light::DrawFrame()
{
	__super::DrawFrame();

	BeginShaderMode(shader);
	// Draw grid for better spatial reference
	DrawGrid(10, 1.0f);

	EndShaderMode();

	// Draw spheres to show where the lights are
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		if (lights[i].enabled) DrawSphereEx(lights[i].position, 0.2f, 8, 8, lights[i].color);
		else DrawSphereWires(lights[i].position, 0.2f, 8, 8, ColorAlpha(lights[i].color, 0.3f));
	}
}

void Demo6Light::DrawGUI()
{
	__super::DrawGUI();

}

