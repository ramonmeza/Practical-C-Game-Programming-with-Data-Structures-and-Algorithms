#include "Demo7Billboard.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main(int argc, char* argv[])
{
	Demo7Billboard* KnightDemo7Billboard = new Demo7Billboard();

	KnightDemo7Billboard->Start();
	KnightDemo7Billboard->GameLoop();

	delete KnightDemo7Billboard;
	return 0;
}

void Demo7Billboard::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	ShowFPS = true;

	//initialize global UI attributes
	GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

	mainCamera = _Scene->CreateSceneObject<FlyThroughCamera>("Chase Camera");

	//Place player
	player = _Scene->CreateSceneObject<SceneActor>("Player");
	player->Scale = Vector3{ 0.3f, 0.3f, 0.3f };
	player->Position = Vector3{ 0.f,0.5f,0.f };
	player->Rotation = Vector3{ 0,0,0 };
	ModelComponent* animPlayerComponent = player->CreateAndAddComponent<ModelComponent>();
	animPlayerComponent->Load3DModel("../../resources/models/gltf/robot.glb");
	animPlayerComponent->SetAnimation(6);

	mainCamera->SetUp(player->Position, 10.0f, 0, 30, 45.0f, CAMERA_PERSPECTIVE);

	//imposter (billboard)
	imposter = _Scene->CreateSceneObject<SceneActor>("Billboard Object");
	imposter->Scale = Vector3{ 1, 1, 1 };
	imposter->Position = Vector3{ 0.f,0.5f,0.f };
	imposter->Rotation = Vector3{ 0,0,0 };
	BillboardComponent* billboard = imposter->CreateAndAddComponent<BillboardComponent>();

	//Load a texture as billboard image
	billboardImage = LoadTexture("../../resources/textures/glow.png");    // Our billboard texture

	//initialize billbard
	billboard->texture = billboardImage;
	// Entire billboard texture, source is used to take a segment from a larger texture.
	billboard->source = { 0.0f, 0.0f, (float)billboard->texture.width, (float)billboard->texture.height };
	billboard->size = { billboard->source.width / billboard->source.height, 1.0f };
	billboard->origin = Vector2Scale(billboard->size, 0.5f);
	billboard->blendingMode = BLEND_ADDITIVE;
	billboard->renderQueue = Component::eRenderQueueType::AlphaBlend;
}

void Demo7Billboard::EndGame()
{
	
	__super::EndGame();
}

void Demo7Billboard::Update(float ElapsedSeconds)
{
	if (IsKeyDown(KEY_W)) {
		// Move player forward based on their rotation
		player->Position.x += sin(DegreesToRadians(player->Rotation.y)) * 0.1f;
		player->Position.z += cos(DegreesToRadians(player->Rotation.y)) * 0.1f;
	}
	if (IsKeyDown(KEY_S)) {
		// Move player backward based on their rotation
		player->Position.x -= sin(DegreesToRadians(player->Rotation.y)) * 0.1f;
		player->Position.z -= cos(DegreesToRadians(player->Rotation.y)) * 0.1f;
	}
	if (IsKeyDown(KEY_A)) {
		player->Rotation.y += 1;  // Rotate left
	}
	if (IsKeyDown(KEY_D)) {
		player->Rotation.y -= 1;  // Rotate right
	}

	mainCamera->SetLookAtPosition(player->Position);

	__super::Update(ElapsedSeconds);
}

void Demo7Billboard::DrawFrame()
{
	__super::DrawFrame();

	// Draw grid for better spatial reference
	DrawGrid(10, 1.0f);
}

void Demo7Billboard::DrawGUI()
{
	__super::DrawGUI();

}

