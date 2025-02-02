#include "Knight.h"
#include "Demo2c.h"

int main(int argc, char* argv[])
{
	Demo2c* KnightDemo2c = new Demo2c();

	KnightDemo2c->Start();
	KnightDemo2c->GameLoop();

	delete KnightDemo2c;
	return 0;
}

void Demo2c::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	ShowFPS = true;

	//Prepare a camera
	camera = _Scene->CreateSceneObject<PerspectiveCamera>("Camera");
	camera->SetPosition(Vector3 { 60, 60, 60 });
	camera->CameraMode = CameraMode::CAMERA_FIRST_PERSON;
	camera->ShowCursor = false;
	camera->SetLookAtPosition(Vector3{ 0, 10, 0 });

	InitEntities();
}

void Demo2c::EndGame()
{
	__super::EndGame();
}

void Demo2c::Update(float ElapsedSeconds)
{
	//process input
	CheckDefeatEnemy();
	player->Update(ElapsedSeconds);
	for(EnemyEntity& enemy : enemies) {
		enemy.Update(ElapsedSeconds);
	}		
	terrain->Update(ElapsedSeconds);
	__super::Update(ElapsedSeconds);
}

void Demo2c::DrawFrame()
{
	__super::DrawFrame();
}

void Demo2c::InitEntities()
{
	terrain = new TerrainEntity();
	terrain->Create(_Scene, NULL);

	player = new PlayerEntity();
	player->Create(_Scene, NULL);

	for (int i = 0; i < 3; i++) {
		EnemyEntity* ee = new EnemyEntity();
		ee->Create(_Scene, NULL);
		enemies.push_back(*ee);
	}

}

void Demo2c::CheckDefeatEnemy()
{
	if (IsKeyPressed(KEY_ONE) && enemies[0].HP > 0)
		enemies[0].Die();
	if (IsKeyPressed(KEY_TWO) && enemies[1].HP > 0)
		enemies[1].Die();
	if (IsKeyPressed(KEY_THREE) && enemies[2].HP > 0)
		enemies[2].Die();	
}
