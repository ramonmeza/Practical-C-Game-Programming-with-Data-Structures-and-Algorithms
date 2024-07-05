// Knight.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "raylib.h"
#include "Knight.h"

Knight::Knight()
	: _Scene(nullptr)
	, _Font()
{

}

Knight::~Knight()
{
}

void Knight::Start()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, APP_TITLE);
	SetTargetFPS(TARGET_FPS);

	_Font = LoadFont("../resources/mecha.png");
	_Scene = new Scene();
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
	while (!WindowShouldClose())
	{
		Update(GetFrameTime());

		BeginDrawing();
		ClearBackground(DARKGRAY);

		PerspectiveCamera* cameraActor = _Scene->GetMainCameraActor();
		if (cameraActor)
		{
			BeginMode3D(cameraActor->_Camera);
			DrawFrame();
			EndMode3D();
		}

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
	_Scene->Update();
}

void Knight::DrawFrame()
{
	_Scene->DrawFrame();
}