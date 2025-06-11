#include "Knight.h"
#include "Demo2b.h"

//Main function for the demo application
int main(int argc, char* argv[])
{
	Demo2b* KnightDemo2b = new Demo2b();

	KnightDemo2b->Start();
	KnightDemo2b->GameLoop();

	delete KnightDemo2b;
	return 0;
}

//Initialize default resources for the demo, like fonts, textures, etc.
void Demo2b::OnCreateDefaultResources()
{
	__super::OnCreateDefaultResources();

	//Replace defaut low resolution font with a TrueType font for the demo
	UnloadFont(_Font);
	_Font = LoadFontEx("../../resources/fonts/sparky.ttf", 32, 0, 0);
}

void Demo2b::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	//Show FPS counter 
	Config.ShowFPS = true;

	//Prepare a camera
	camera = _Scene->CreateSceneObject<PerspectiveCamera>("Camera");
	camera->SetPosition(Vector3{ 60, 60, 60 });
	camera->CameraMode = CameraMode::CAMERA_THIRD_PERSON;
	camera->ShowCursor = false;
	camera->SetLookAtPosition(Vector3{0, 3, 0});

	InitEntities();
}

void Demo2b::Update(float ElapsedSeconds)
{
	//process input and update Entitie objects
	CheckDefeatEnemy();
	player->Update(ElapsedSeconds);
	for(int i=0;i< MAX_NUM_ENEMIES; i++)
		enemies[i].Update(ElapsedSeconds);
	terrain->Update(ElapsedSeconds);

	//Don't forget to call parent class Update method so Scene can be updated too.
	__super::Update(ElapsedSeconds);
}

//Initialize entities for the demo
void Demo2b::InitEntities()
{
	terrain = new TerrainEntity();
	terrain->Create(_Scene, NULL);

	player = new PlayerEntity();
	player->Create(_Scene, NULL);

	for(int i=0; i< MAX_NUM_ENEMIES; i++)
		enemies[i].Create(_Scene, NULL);
}

//Check if player has defeated an enemy by pressing 1, 2 or 3 keys
bool Demo2b::CheckDefeatEnemy()
{
	if (IsKeyPressed(KEY_ONE) && enemies[0].HP > 0)
		enemies[0].Die();
	else if (IsKeyPressed(KEY_TWO) && enemies[1].HP > 0)
		enemies[1].Die();
	else if (IsKeyPressed(KEY_THREE) && enemies[2].HP > 0)
		enemies[2].Die();
	else
		return false;

	return true;
}

//Render help instruction text on the screen
void Demo2b::DrawGUI()
{
	DrawText("Press 1 to defeat 1st enemy, 2 to defeat 2nd enemy, 3 to defeat 3rd enemy!", 10, 50, 40, WHITE);
	DrawText("Rotate camera angle: arrow keys.", 10, 100, 40, WHITE);
	DrawText("Zoom Camera: mouse wheel.", 10, 150, 40, WHITE);
}
