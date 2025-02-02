#include "Demo6NormalMap.h"

#include "raymath.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <cmath>

#include "rlgl.h"

int main(int argc, char* argv[])
{
	Demo6NormalMap* KnightDemo6NormalMap = new Demo6NormalMap();

	KnightDemo6NormalMap->Start();
	KnightDemo6NormalMap->GameLoop();

	delete KnightDemo6NormalMap;
	return 0;
}

Demo6NormalMap::Demo6NormalMap()
{
}

void Demo6NormalMap::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	ShowFPS = true;

	//initialize global UI attributes
	GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

	SetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)

	pMainCamera = _Scene->CreateSceneObject<FlyThroughCamera>("Main Camera");
	pMainCamera->SetUp(Vector3{ 0.0f, 3.0f, 0.0f }, 8.0f, 0, 30, 45, CAMERA_PERSPECTIVE);

	// Load model, diffuse texture, and normal map
	model = LoadModel("../../resources/models/obj/cylinder.obj");            // Replace with your model path
	diffuse = LoadTexture("../../resources/models/obj/wall_diffuse.png");  // Replace with your diffuse texture path
	normalMap = LoadTexture("../../resources/models/obj/wall_normal.png"); // Replace with your normal map texture path
	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = diffuse;
	model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = normalMap;
	for (int i = 0; i < model.meshCount; i++) {
		GenMeshTangents(&model.meshes[i]);
	}

	model2 = LoadModel("../../resources/models/obj/cylinder.obj");
	model2.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = diffuse;
	model2.materials[0].maps[MATERIAL_MAP_NORMAL].texture = normalMap;
	for (int i = 0; i < model2.meshCount; i++) {
		GenMeshTangents(&model2.meshes[i]);
	}

	// Load shaders
	shader = LoadShader("../../resources/shaders/glsl330/normalmap.vs", 
		"../../resources/shaders/glsl330/normalmap.fs");
	model.materials[0].shader = shader;
	model2.materials[0].shader = shader;

	// Get shader uniform locations
	lightPosLoc = GetShaderLocation(shader, "LightPosition_worldspace");	
	SetShaderValue(shader, lightPosLoc, &lightPos, SHADER_UNIFORM_VEC3);
	lightPowerLoc = GetShaderLocation(shader, "LightPower");
	SetShaderValue(shader, lightPowerLoc, &lightPower, SHADER_UNIFORM_FLOAT);
	lightColorLoc = GetShaderLocation(shader, "LightColor");
	SetShaderValue(shader, lightColorLoc, &lightColor, SHADER_UNIFORM_VEC3);
}

void Demo6NormalMap::EndGame()
{
	// Unload resources
	UnloadModel(model);
	UnloadTexture(diffuse);
	UnloadTexture(normalMap);
	UnloadShader(shader);
	__super::EndGame();
}

void Demo6NormalMap::Update(float ElapsedSeconds)
{
	if (IsKeyDown(KEY_W)) {
		lightPos.z += ElapsedSeconds;
	}
	if (IsKeyDown(KEY_S)) {
		// Move player backward based on their rotation
		lightPos.z -= ElapsedSeconds;
	}
	if (IsKeyDown(KEY_A)) {
		lightPos.x -= ElapsedSeconds;  // left
	}
	if (IsKeyDown(KEY_D)) {
		lightPos.x += ElapsedSeconds;  // right
	}
	SetShaderValue(shader, lightPosLoc, &lightPos, SHADER_UNIFORM_VEC3);

	model.transform = MatrixRotateXYZ(Vector3 { 0.0f, (float)GetTime(), 0.0f });

	__super::Update(ElapsedSeconds);
}

void Demo6NormalMap::DrawFrame()
{
	DrawModel(model, modelpos, 1.0f, WHITE);
	DrawModel(model2, modelpos2, 1.0f, BLUE);

	__super::DrawFrame();

	// Draw light as a small sphere
	DrawSphere(lightPos, 0.1f, YELLOW);
}

void Demo6NormalMap::DrawGUI()
{
	__super::DrawGUI();

	char buf[80];

	Vector3 v = pMainCamera->GetPosition();
	sprintf_s(buf, sizeof(buf), "cam: %f, %f, %f", v.x, v.y, v.z);

	DrawText(buf, 100, 100, 24, WHITE);
}

