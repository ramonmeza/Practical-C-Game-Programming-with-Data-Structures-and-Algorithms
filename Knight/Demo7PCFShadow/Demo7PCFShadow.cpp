#include "Demo7PCFShadow.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main(int argc, char* argv[])
{
	Demo7PCFShadow* KnightDemo7PCFShadow = new Demo7PCFShadow();
	KnightDemo7PCFShadow->Start();
	KnightDemo7PCFShadow->GameLoop();
	delete KnightDemo7PCFShadow;
	return 0;
}

Demo7PCFShadow::Demo7PCFShadow()
{
}

void Demo7PCFShadow::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();
	ShowFPS = true;

	//initialize global UI attributes
	GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
	
	pMainCamera = _Scene->CreateSceneObject<FlyThroughCamera>("Main Camera");
	pMainCamera->SetUp(Vector3{ 0,0,0 }, 60, 0, 60, 45, CAMERA_PERSPECTIVE);
	
	sceneLight = _Scene->CreateSceneObject<ShadowSceneLight>("Light");
	sceneLight->SetLight(Vector3{ 0.35f, -1.0f, -0.35f }, WHITE);
	
	Floor = _Scene->CreateSceneObject<SceneActor>("Floor");
	Floor->Scale = Vector3{ 1.0f, 1.0f, 1.0f };
	Floor->Position = Vector3{ 0.f,0.0f,0.f };
	Floor->Rotation = Vector3{ 0,0,0 };
	
	CubeComponent* floor = new CubeComponent();
	floor->Size = Vector3{ 10, 1, 10 };
	floor->SetColor(BLUE);
	Floor->AddComponent(floor);
	
	Box = _Scene->CreateSceneObject<SceneActor>("Box");
	Box->Scale = Vector3{ 1.0f, 1.0f, 1.0f };
	Box->Position = Vector3{ 2.f,0.0f,2.f };
	Box->Rotation = Vector3{ 0,0,0 };
	
	CubeComponent* box = new CubeComponent();
	box->Size = Vector3{ 3, 3, 3 };
	box->SetColor(WHITE);
	Box->AddComponent(box);
	
	//Place player
	Actor = _Scene->CreateSceneObject<SceneActor>("Player");
	Actor->Scale = Vector3{ 0.3f, 0.3f, 0.3f };
	Actor->Position = Vector3{ 0.f,0.5f,0.f };
	Actor->Rotation = Vector3{ 0,0,0 };
	ModelComponent* animPlayerComponent = Actor->CreateAndAddComponent<ModelComponent>();
	animPlayerComponent->Load3DModel("../../resources/models/gltf/robot.glb");
	animPlayerComponent->SetAnimation(0);
	animPlayerComponent->castShadow = Component::eShadowCastingType::Shadow;
	animPlayerComponent->receiveShadow = true;
	pDepthRenderer = new DepthRenderPass(sceneLight);
	pDepthRenderer->Create(_Scene);

	pShadowMapRenderer = new ShadowMapRenderPass(sceneLight, pDepthRenderer->shadowMap.depth.id);
	pShadowMapRenderer->Create(_Scene);
}

void Demo7PCFShadow::EndGame()
{
	UnloadModelAnimations(robotAnimations, animCount);
	__super::EndGame();
}

void Demo7PCFShadow::Update(float ElapsedSeconds)
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

	Vector3 cameraPos = _Scene->GetMainCameraActor()->GetPosition();
	SetShaderValue(pShadowMapRenderer->shadowShader, pShadowMapRenderer->shadowShader.locs[SHADER_LOC_VECTOR_VIEW], &cameraPos, SHADER_UNIFORM_VEC3);
	float dt = GetFrameTime();
	if (IsKeyDown(KEY_J))
	{
		if (sceneLight->lightDir.x < 0.6f)
			sceneLight->lightDir.x += cameraSpeed * 60.0f * dt;
	}

	if (IsKeyDown(KEY_K))
	{
		if (sceneLight->lightDir.x > -0.6f)
			sceneLight->lightDir.x -= cameraSpeed * 60.0f * dt;
	}

	if (IsKeyDown(KEY_I))
	{
		if (sceneLight->lightDir.z < 0.6f)
			sceneLight->lightDir.z += cameraSpeed * 60.0f * dt;
	}

	if (IsKeyDown(KEY_M))
	{
		if (sceneLight->lightDir.z > -0.6f)
			sceneLight->lightDir.z -= cameraSpeed * 60.0f * dt;
	}

	sceneLight->lightDir = Vector3Normalize(sceneLight->lightDir);
	pDepthRenderer->lightCam.position = Vector3Scale(sceneLight->lightDir, -15.0f);
	SetShaderValue(pShadowMapRenderer->shadowShader, pShadowMapRenderer->lightDirLoc, &sceneLight->lightDir, SHADER_UNIFORM_VEC3);
	__super::Update(ElapsedSeconds);  //allow update main camera
}

void Demo7PCFShadow::DrawOffscreen()
{
	pDepthRenderer->BeginShadowMap(_Scene);
	pDepthRenderer->BeginScene();
	pDepthRenderer->Render();
	pDepthRenderer->EndScene();
	pDepthRenderer->EndShadowMap();
}

void Demo7PCFShadow::DrawFrame()
{
	pShadowMapRenderer->BeginScene();
	__super::DrawFrame();
	pShadowMapRenderer->EndScene();
}

void Demo7PCFShadow::DrawGUI()
{
	Rectangle sr = { 0,0,1023, 1023 };
	Rectangle dr = { 0,0,255, 255 };
	Vector2 pos = { SCREEN_WIDTH - 300, 300 };
	DrawTextureEx(pDepthRenderer->shadowMap.depth, pos, 0, 0.25f, WHITE);
	__super::DrawGUI();
	DrawText("Shadows in raylib using the shadowmapping algorithm!", SCREEN_WIDTH - 320, SCREEN_HEIGHT - 20, 10, GRAY);
	DrawText("Use the arrow keys to rotate the light!", 10, 10, 30, RED);
}
