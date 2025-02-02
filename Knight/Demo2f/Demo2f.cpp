#include "Knight.h"
#include "Demo2f.h"

#include <time.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main(int argc, char* argv[])
{
	Demo2f* KnightDemo2f = new Demo2f();

	KnightDemo2f->Start();
	KnightDemo2f->GameLoop();

	delete KnightDemo2f;
	return 0;
}

std::list<std::string> Demo2f::messages;
GameResult Demo2f::_gameOver = InProgress;

void Demo2f::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	srand(time(NULL));

	ShowFPS = true;

	//Prepare a camera
	camera = _Scene->CreateSceneObject<PerspectiveCamera>("Camera");
	camera->CameraMode = CameraMode::CAMERA_FIRST_PERSON;
	camera->ShowCursor = false;
	camera->SetPosition(Vector3{ 60, 60, 60 });
	camera->SetLookAtPosition(Vector3{0, 10, 0});

	InitEntities();

	//initialize global UI attributes
	GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

	Log("Start the game!");
}

void Demo2f::EndGame()
{
	__super::EndGame();
}

void Demo2f::Update(float ElapsedSeconds)
{
	//process input
	player->Update(ElapsedSeconds);
	enemy->Update(ElapsedSeconds);
	terrain->Update(ElapsedSeconds);
	__super::Update(ElapsedSeconds);
}

void Demo2f::DrawFrame()
{
	__super::DrawFrame();
}

void Demo2f::DrawGUI()
{
	player->DrawGUI();
	enemy->DrawGUI();

	switch (_gameOver)
	{
		case InProgress:
		{
			int line = 0;
			for (const auto& msg : messages) {
				DrawText(msg.c_str(), 150, 150 + 40 * line, 25, WHITE);
				++line;
			}
			break;
		}
		case YouWin:
		{
			Rectangle r;
			int result = GuiMessageBox(CenterRectangle(r, 400, 400),
				"#191#Congrate!", "You Win!", "Nice!");
			if (result > 0 || IsKeyPressed(KEY_ENTER))
				exit(0);
			break;
		}
		case YouLose:
		{
			Rectangle r;
			int result = GuiMessageBox(CenterRectangle(r, 400, 400),
				"#191#Oh No!", "You Lose!", "Okay");
			if (result > 0 || IsKeyPressed(KEY_ENTER))
				exit(0);
			break;
		}
	}

}

void Demo2f::InitEntities()
{
	terrain = new TerrainEntity();
	terrain->Create(_Scene, NULL);

	player = new PlayerEntity();
	player->Create(_Scene, NULL);
	
	enemy = new EnemyEntity();
	enemy->Create(_Scene, NULL);

	player->SetTarget(enemy);
	enemy->SetTarget(player);

}

void Demo2f::GameOver(GameResult gr)
{
	_gameOver = gr;
}

void Demo2f::Log(const std::string& message) {
	if (messages.size() >= 10) {
		messages.pop_front();  // Remove the oldest message
	}
	messages.push_back(message);  // Add the new message
}

