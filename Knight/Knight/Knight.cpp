// Knight.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "raylib.h"
#include "Knight.h"

#include "rlgl.h"

Knight* Knight::Instance = nullptr;

Knight::Knight()
	: _Scene(nullptr)
	, _Font()
	, _shouldExitGameLoop(false)
{
	Instance = this;
}

Knight::~Knight()
{
}

void Knight::OnCreateDefaultResources()
{
	_Font = LoadFont("../../resources/fonts/mecha.png");

	ForwardRenderPass* pLightPass = nullptr;

	pLightPass = new ForwardRenderPass();
	pLightPass->Create(_Scene);

	_Scene->_CurrentRenderPass = pLightPass;

	//Set the default light data
	pLightPass->Lights[0].enabled = true;
	pLightPass->Lights[0].color = WHITE;
	pLightPass->Lights[0].position = Vector3{ 20.0f, 50.0f, 50.0f };
	pLightPass->Lights[0].target = Vector3{ 0.0f, 0.0f, 0.0f };
	pLightPass->Lights[0].dirty = true;
}

void Knight::OnReleaseDefaultResources()
{
}

void Knight::Start()
{
	OnConfigKnightApp();

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, APP_TITLE);
	SetTargetFPS(TARGET_FPS);

	_Scene = new Scene();

	OnCreateDefaultResources();
}

void Knight::EndGame()
{
	OnReleaseDefaultResources();

	UnloadFont(_Font);

	delete _Scene;
	_Scene = nullptr;

	CloseWindow();
}

void Knight::GameLoop()
{
	while (!WindowShouldClose() && (!_shouldExitGameLoop))
	{	
		Update(GetFrameTime());

		DrawOffscreen();

		BeginDrawing();
		ClearBackground(DARKGRAY);
		SceneCamera* cameraActor = _Scene->GetMainCameraActor();
		if (cameraActor)
		{
			BeginMode3D(*cameraActor->GetCamera3D());
			DrawFrame();
			EndMode3D();
		}
		else {
			DrawFrame();
		}

		DrawGUI();

		if (Config.ShowFPS)
		{
			DrawFPS(10, 10);
		}

		EndDrawing();
	}

	EndGame();
}

void Knight::Update(float ElapsedSeconds)
{
	_Scene->Update(ElapsedSeconds);
}

void Knight::DrawFrame()
{
	_Scene->DrawFrame();
}

void Knight::DrawOffscreen()
{

}

void Knight::DrawGUI()
{

}

void Knight::ExitGameLoop()
{
	_shouldExitGameLoop = true;
}

void Knight::DrawFPS(int x, int y)
{
	char text[32];
	sprintf_s(text, "FPS: %i", GetFPS());
	DrawTextEx(_Font, text, Vector2{ (float)x, (float)y }, 40, 3, WHITE);
}

void Knight::DrawText(const char* text, int x, int y, int size, const Color& color)
{
	DrawTextEx(_Font, text, Vector2{ (float)x, (float)y }, size, 3, color);
}

void Knight::OnConfigKnightApp()
{

}

void Knight::SaveScreenshot(const char* fileName)
{
	// Security check to (partially) avoid malicious code
	if (strchr(fileName, '\'') != NULL) 
	{ 
		printf("SYSTEM: Provided fileName could be potentially malicious, avoid [\'] character"); 
		return; 
	}

	unsigned char* imgData = rlReadScreenPixels(SCREEN_WIDTH, SCREEN_HEIGHT);
	Image image = { imgData, SCREEN_WIDTH, SCREEN_HEIGHT, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };

	char path[512] = { 0 };
	strcpy_s(path, TextFormat("%s/%s", "./", GetFileName(fileName)));

	ExportImage(image, path);           // WARNING: Module required: rtextures
	RL_FREE(imgData);

	if (FileExists(path)) 
		TRACELOG(LOG_INFO, "SYSTEM: [%s] Screenshot taken successfully", path);
	else 
		TRACELOG(LOG_WARNING, "SYSTEM: [%s] Screenshot could not be saved", path);
}
