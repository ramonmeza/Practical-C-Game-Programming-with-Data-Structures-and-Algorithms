#include "Knight.h"
#include "Demo4iso.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main(int argc, char* argv[])
{
	Demo4iso* KnightDemo4iso = new Demo4iso();

	KnightDemo4iso->Start();
	KnightDemo4iso->GameLoop();

	delete KnightDemo4iso;
	return 0;
}

Demo4iso::Demo4iso()
{
	camera = NULL;
	for (int y = 0; y < 10; ++y) {
		for (int x = 0; x < 10; ++x) {
			map[x][y] = rand() % 96; // Example: Initialize all tiles as empty
		}
	}
}

void Demo4iso::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	ShowFPS = true;

	Image image = LoadImage("../../resources/textures/seasons_tiles.png");     // Loaded in CPU memory (RAM)
	_mapTiles = LoadTextureFromImage(image);          // Image converted to texture, GPU memory (VRAM)
	UnloadImage(image);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

	//initialize global UI attributes
	GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
}

void Demo4iso::EndGame()
{
	UnloadTexture(_mapTiles);
	__super::EndGame();
}

void Demo4iso::Update(float ElapsedSeconds)
{
	//process input
	__super::Update(ElapsedSeconds);
}

void Demo4iso::DrawFrame()
{
	__super::DrawFrame();
}

void Demo4iso::DrawGUI()
{
	for (int y = 0; y < 10; ++y) {
		for (int x = 0; x < 10; ++x) {
			// Convert 2D grid coordinates to isometric screen coordinates
			int screenX = (x - y) * 128 / 2+ SCREEN_WIDTH/2;
			int screenY = (x + y) * 64 / 2+ SCREEN_HEIGHT/4;

			// Draw the tile at (screenX, screenY)
			//DrawCircleLines(screenX, screenY, 50, WHITE);
			Rectangle r = {(map[x][y] % 8)*128,(map[x][y]/12)*64, 128, 64};
			Vector2 v2;
			v2.x = screenX; v2.y = screenY;
			DrawTextureRec(_mapTiles, r, v2, WHITE);
		}
	}
}



