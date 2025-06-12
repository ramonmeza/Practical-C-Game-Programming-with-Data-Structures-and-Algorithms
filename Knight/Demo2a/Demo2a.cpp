#include "Knight.h"
#include "Demo2a.h"

//main entry point for the application
int main(int argc, char* argv[])
{
	Demo2a* KnightDemo2a = new Demo2a();

	KnightDemo2a->Start();
	KnightDemo2a->GameLoop();

	delete KnightDemo2a;
	return 0;
}

void Demo2a::OnCreateDefaultResources()
{
	__super::OnCreateDefaultResources();

	//Loads a better TrueType font to display text information on the screen
	UnloadFont(_Font);
	_Font = LoadFontEx("../../resources/fonts/sparky.ttf", 32, 0, 0);
}

void Demo2a::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	Config.ShowFPS = true;

	//Prepare a built-in camera and set its properties
	camera = _Scene->CreateSceneObject<PerspectiveCamera>("Camera");
	camera->SetPosition(Vector3 { 60, 60, 60 });
	camera->CameraMode = CameraMode::CAMERA_THIRD_PERSON;
	camera->ShowCursor = false;
	camera->SetLookAtPosition(Vector3{0, 3, 0});

	//Create entities, refer to the introduction of Entity in Chapter 2.
	InitEntities();
}

void Demo2a::Update(float ElapsedSeconds)
{
	player->Update(ElapsedSeconds);
	enemy->Update(ElapsedSeconds);
	prop->Update(ElapsedSeconds);
	terrain->Update(ElapsedSeconds);
	__super::Update(ElapsedSeconds);
}

void Demo2a::DrawFrame()
{
	__super::DrawFrame();
}

void Demo2a::InitEntities()
{
	//First create a terrain entity, which is the ground of the scene.
	terrain = new TerrainEntity();
	terrain->Create(_Scene, NULL);

	//Create a player and an enemy entity on the terrain
	player = new PlayerEntity();
	player->Create(_Scene, terrain);

	//Create an prop Eneity 
	prop = new PropEntity();
	prop->Create(_Scene, terrain);

	//Create an emeny Entity
	enemy = new EnemyEntity();
	enemy->Create(_Scene, terrain);	
}

void Demo2a::DrawGUI()
{
	DrawText("Rotate camera angle by mouse or arrow keys.", 10, 50, 40, WHITE);
	DrawText("Zoom Camera by mouse wheel.", 10, 100, 40, WHITE);
}