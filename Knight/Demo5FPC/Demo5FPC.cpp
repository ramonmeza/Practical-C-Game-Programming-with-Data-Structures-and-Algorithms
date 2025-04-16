#include "Demo5FPC.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main(int argc, char* argv[])
{
	Demo5FPC* KnightDemo5FPC = new Demo5FPC();

	KnightDemo5FPC->Start();
	KnightDemo5FPC->GameLoop();

	delete KnightDemo5FPC;
	return 0;
}

Demo5FPC::Demo5FPC()
{
	FPSCamera = NULL;
}

void Demo5FPC::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	ShowFPS = true;

	//initialize global UI attributes
	GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

	//initialize a first person camera
	FPSCamera = _Scene->CreateSceneObject<PerspectiveCamera>("Camera");
	FPSCamera->SetPosition(Vector3{0.0f, 2.0f, 4.0f});    // Camera position
	FPSCamera->SetLookAtPosition(Vector3{ 0.0f, 2.0f, 0.0f });      // Camera looking at point
	FPSCamera->SetFovY(60.0f);                                // Camera field-of-view Y	
	FPSCamera->CameraMode = CAMERA_FIRST_PERSON;   

	for (int i = 0; i < MAX_COLUMNS; i++)
	{
		heights[i] = (float)GetRandomValue(1, 12);
		positions[i] = Vector3{ (float)GetRandomValue(-15, 15), heights[i] / 2.0f, (float)GetRandomValue(-15, 15) };
		colors[i] = Color{ (unsigned char)GetRandomValue(20, 255), (unsigned char)GetRandomValue(10, 55), 30, 255 };
	}

	DisableCursor();
}

void Demo5FPC::Update(float elaspedTime)
{
	UpdateCamera(FPSCamera->GetCamera3D(), CAMERA_FIRST_PERSON);
}

void Demo5FPC::DrawFrame()
{
	DrawPlane(Vector3{ 0.0f, 0.0f, 0.0f }, Vector2{ 32.0f, 32.0f }, LIGHTGRAY); // Draw ground
	DrawCube(Vector3{ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
	DrawCube(Vector3{ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
	DrawCube(Vector3{ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      // Draw a yellow wall

	for (int i = 0; i < MAX_COLUMNS; i++)
	{
		DrawCube(positions[i], 2.0f, heights[i], 2.0f, colors[i]);
		DrawCubeWires(positions[i], 2.0f, heights[i], 2.0f, MAROON);
	}

	__super::DrawFrame();
}

void Demo5FPC::DrawGUI()
{
	__super::DrawGUI();


}
