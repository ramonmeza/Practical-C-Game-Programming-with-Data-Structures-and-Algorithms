#include "Knight.h"
#include "Demo2b.h"

int main(int argc, char* argv[])
{
	Demo2b* KnightDemo2b = new Demo2b();

	KnightDemo2b->Start();
	KnightDemo2b->GameLoop();

	delete KnightDemo2b;
	return 0;
}

void Demo2b::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	ShowFPS = true;

	//Prepare a camera
	camera = _Scene->CreateSceneObject<PerspectiveCamera>("Camera");
	camera->SetPosition(Vector3{ 60, 60, 60 });
	camera->CameraMode = CameraMode::CAMERA_CUSTOM;
	camera->ShowCursor = false;
	camera->SetLookAtPosition(Vector3{0, 10, 0});

	InitEntities();
}

void Demo2b::EndGame()
{
	__super::EndGame();
}

void Demo2b::Update(float ElapsedSeconds)
{
	//process input
	CheckDefeatEnemy();
	player->Update(ElapsedSeconds);
	for(int i=0;i< MAX_NUM_ENEMIES; i++)
		enemies[i].Update(ElapsedSeconds);
	terrain->Update(ElapsedSeconds);
	__super::Update(ElapsedSeconds);
}

void Demo2b::DrawFrame()
{
	__super::DrawFrame();
}

void Demo2b::InitEntities()
{
	terrain = new TerrainEntity();
	terrain->Create(_Scene, NULL);

	player = new PlayerEntity();
	player->Create(_Scene, NULL);

	for(int i=0; i< MAX_NUM_ENEMIES; i++)
		enemies[i].Create(_Scene, NULL);
}

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
