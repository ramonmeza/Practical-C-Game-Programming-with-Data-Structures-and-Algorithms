// Knight.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "raylib.h"
#include "Knight.h"

Knight::Knight()
	: _Scene(nullptr)
	, _Font()
	, _shouldExitGameLoop(false)
{

}

Knight::~Knight()
{
}

void Knight::Start()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, APP_TITLE);
	SetTargetFPS(TARGET_FPS);

	_Font = LoadFont("../../resources/mecha.png");
	_Scene = new Scene();

	pDefaultRenderer = new ForwardRenderPass();
	pDefaultRenderer->Create(_Scene);

	_Scene->_CurrentRenderPass = pDefaultRenderer;
}

void Knight::EndGame()
{
	UnloadFont(_Font);
	delete _Scene;
	_Scene = nullptr;
	CloseWindow();
}

void Knight::GameLoop()
{
	Vector2 v;

	while (!WindowShouldClose() && (!_shouldExitGameLoop))
	{	
		v = GetMousePosition();
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

		if (ShowFPS)
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