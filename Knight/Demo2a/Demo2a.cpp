#include "Knight.h"
#include "Demo2a.h"

int main(int argc, char* argv[])
{
	Demo2a* KnightDemo2a = new Demo2a();

	KnightDemo2a->Start();
	KnightDemo2a->GameLoop();

	delete KnightDemo2a;
	return 0;
}

void Demo2a::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	ShowFPS = true;

	//Prepare a camera
	camera = _Scene->CreateSceneObject<PerspectiveCamera>("Camera");
	camera->SetPosition(Vector3 { 60, 60, 60 });
	camera->CameraMode = CameraMode::CAMERA_CUSTOM;
	camera->ShowCursor = false;
	camera->SetLookAtPosition(Vector3{0, 10, 0});

	InitEntities();
}

void Demo2a::EndGame()
{
	__super::EndGame();
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
	terrain = new TerrainEntity();
	terrain->Create(_Scene, NULL);

	prop = new PropEntity();
	prop->Create(_Scene, terrain);

	player = new PlayerEntity();
	player->Create(_Scene, terrain);

	enemy = new EnemyEntity();
	enemy->Create(_Scene, terrain);	
}
