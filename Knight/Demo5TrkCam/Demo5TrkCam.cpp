#include "Demo5TrkCam.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main(int argc, char* argv[])
{
	Demo5TrkCam* KnightDemo5TrkCam = new Demo5TrkCam();

	KnightDemo5TrkCam->Start();
	KnightDemo5TrkCam->GameLoop();

	delete KnightDemo5TrkCam;
	return 0;
}

Demo5TrkCam::Demo5TrkCam()
{
	OnRailCamera = NULL;
}

void Demo5TrkCam::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	ShowFPS = true;

	//initialize global UI attributes
	GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

	//initialize a first person camera
	OnRailCamera = _Scene->CreateSceneObject<WaypointsCamera>("Raill Camera");

	//Place player
	Actor = _Scene->CreateSceneObject<SceneActor>("Player");
	Actor->Scale = Vector3{ 0.3f, 0.3f, 0.3f };
	Actor->Position = Vector3{ -10.f,0.15f,0.f };
	Actor->Rotation = Vector3{ 0,90,0 };
	ModelComponent* animPlayerComponent = Actor->CreateAndAddComponent<ModelComponent>();
	animPlayerComponent->Load3DModel("../../resources/models/gltf/robot.glb");
	animPlayerComponent->SetAnimation(6);
	Actor->AddComponent(animPlayerComponent);

	OnRailCamera->SetUp(Actor, 45.0f, CAMERA_PERSPECTIVE);

	DisableCursor();
}

void Demo5TrkCam::EndGame()
{
	__super::EndGame();
}

void Demo5TrkCam::Update(float ElapsedSeconds)
{
	float actorSpeed = 10.0f;

	// Update cube position along a straight path
	cubePosition.x += cubeDirection.x * ElapsedSeconds * actorSpeed;
	// Reverse direction when it reaches bounds
	if (cubePosition.x > 10.0f || cubePosition.x < -10.0f) {
		cubeDirection.x = -cubeDirection.x;
	}
	Actor->Position = cubePosition;

	__super::Update(ElapsedSeconds);

}

void Demo5TrkCam::DrawFrame()
{
	OnRailCamera->Draw();
	DrawGrid(50, 3.0f);
	__super::DrawFrame();
}

void Demo5TrkCam::DrawGUI()
{
	__super::DrawGUI();


}
