#include "Demo5Ortho.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <ctime>

int main(int argc, char* argv[])
{
	Demo5Ortho* KnightDemo5Ortho = new Demo5Ortho();

	KnightDemo5Ortho->Start();
	KnightDemo5Ortho->GameLoop();

	delete KnightDemo5Ortho;
	return 0;
}

Demo5Ortho::Demo5Ortho()
{
	srand(static_cast<unsigned int>(time(nullptr)));
}

void Demo5Ortho::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	ShowFPS = true;

	//initialize global UI attributes
	GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

	OrthCam = _Scene->CreateSceneObject<OrthogonalCamera>("Orthogonal Camera");
	OrthCam->SetUp(Vector3{ 0.0f, 15.0f, 15.0f }, Vector3{ 0.0f, 0.0f, 0.0f }, 20.0f);
}

void Demo5Ortho::EndGame()
{
	__super::EndGame();
}

void Demo5Ortho::Update(float ElapsedSeconds)
{
	__super::Update(ElapsedSeconds);
}

void Demo5Ortho::DrawFrame()
{
	__super::DrawFrame();

	DrawGrid(20, 1.0f);

	Vector3 pos = OrthCam->GetCamera3D()->target;
	DrawCube(pos, 1.0f, 1.0f, 1.0f, PURPLE);
	DrawCubeWires(pos, 1.0f, 1.0f, 1.0f, YELLOW);
}

void Demo5Ortho::DrawGUI()
{
	__super::DrawGUI();
}
