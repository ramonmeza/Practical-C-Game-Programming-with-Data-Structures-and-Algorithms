#include "Demo6Particle.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

// Define particle count
#define PARTICLE_COUNT 500

int main(int argc, char* argv[])
{
	Demo6Particle* KnightDemo6Particle = new Demo6Particle();

	KnightDemo6Particle->Start();
	KnightDemo6Particle->GameLoop();

	delete KnightDemo6Particle;
	return 0;
}

Demo6Particle::Demo6Particle()
{
}

void Demo6Particle::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	ShowFPS = true;

	//initialize global UI attributes
	GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

	pMainCamera = _Scene->CreateSceneObject<FlyThroughCamera>("Main Camera");
	pMainCamera->SetUp(Vector3{ 0.0f, 0.5f, 0.0f }, 10.0f, 0, 30, 45.0f, CAMERA_PERSPECTIVE);

	//Place player
	Actor = _Scene->CreateSceneObject<SceneActor>("Player");
	Actor->Scale = Vector3{ 0.3f, 0.3f, 0.3f };
	Actor->Position = Vector3{ 0.f,0.5f,0.f };
	Actor->Rotation = Vector3{ 0,0,0 };
	ModelComponent* animPlayerComponent = Actor->CreateAndAddComponent<ModelComponent>();
	animPlayerComponent->Load3DModel("../../resources/models/gltf/robot.glb");
	animPlayerComponent->SetAnimation(6);

	// Set up particle system
	pParticle = Actor->CreateAndAddComponent<ParticleComponent>();
	pParticle->CreateFromFile("../../resources/textures/glow.png", PARTICLE_COUNT, fountainOrigin, Color{ 255, 150, 50, 255 }, Vector3{0,0,0});

}

void Demo6Particle::EndGame()
{
	__super::EndGame();
}

void Demo6Particle::Update(float ElapsedSeconds)
{
	if (IsKeyDown(KEY_W)) {
		// Move player forward based on their rotation
		Actor->Position.x += sin(DegreesToRadians(Actor->Rotation.y)) * 0.1f;
		Actor->Position.z += cos(DegreesToRadians(Actor->Rotation.y)) * 0.1f;
	}
	if (IsKeyDown(KEY_S)) {
		// Move player backward based on their rotation
		Actor->Position.x -= sin(DegreesToRadians(Actor->Rotation.y)) * 0.1f;
		Actor->Position.z -= cos(DegreesToRadians(Actor->Rotation.y)) * 0.1f;
	}
	if (IsKeyDown(KEY_A)) {
		Actor->Rotation.y += 1;  // Rotate left
	}
	if (IsKeyDown(KEY_D)) {
		Actor->Rotation.y -= 1;  // Rotate right
	}

	__super::Update(ElapsedSeconds);
}

void Demo6Particle::DrawFrame()
{
	DrawGrid(10, 1);

	__super::DrawFrame();
}

void Demo6Particle::DrawGUI()
{
	__super::DrawGUI();

}

