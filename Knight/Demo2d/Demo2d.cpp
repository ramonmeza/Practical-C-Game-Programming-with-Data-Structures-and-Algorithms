#include "Knight.h"
#include "Demo2d.h"

int main(int argc, char* argv[])
{
	Demo2d* KnightDemo2d = new Demo2d();

	KnightDemo2d->Start();
	KnightDemo2d->GameLoop();

	delete KnightDemo2d;
	return 0;
}

void Demo2d::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	ShowFPS = true;

	//Prepare a camera
	camera = _Scene->CreateSceneObject<PerspectiveCamera>("Camera");
	camera->SetPosition(Vector3 { 60, 60, 60 });
	camera->CameraMode = CameraMode::CAMERA_CUSTOM;
	camera->ShowCursor = false;
	camera->SetLookAtPosition( Vector3{ 0, 10, 0 });

	InitEntities();
}

void Demo2d::EndGame()
{
	__super::EndGame();
}

void Demo2d::Update(float ElapsedSeconds)
{
	//process input
	CheckDefeatEnemy();
	player->Update(ElapsedSeconds);
	for (EnemyEntity& enemy : enemies) {
		enemy.Update(ElapsedSeconds);
	}
	terrain->Update(ElapsedSeconds);
	__super::Update(ElapsedSeconds);
}

void Demo2d::DrawFrame()
{
	__super::DrawFrame();
}

void Demo2d::InitEntities()
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

void Demo2d::CheckDefeatEnemy()
{
	list<EnemyEntity>::iterator it = enemies.begin();	
	if (IsKeyPressed(KEY_ONE) && it->HP > 0)
		it->Die();
	++it;
	if (IsKeyPressed(KEY_TWO) && it->HP > 0)
		it->Die();
	++it;
	if (IsKeyPressed(KEY_THREE) && it->HP > 0)
		it->Die();	
}

