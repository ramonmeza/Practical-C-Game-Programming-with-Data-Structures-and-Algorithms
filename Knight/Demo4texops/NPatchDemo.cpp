#include "Demo4TexOps.h"

#include <cmath>

NPatchDemo::NPatchDemo()
{
	title = "N-Patch Demo";
	description = "This demo draws UI frame with N-patch texture redering.\nPress Enter to continue.";
	elapsed_time = 0;
}

void NPatchDemo::Create()
{
	Image art = LoadImage("../../resources/textures/uibkgd.png");
	_npatchTex = LoadTextureFromImage(art);
	UnloadImage(art);

	Rectangle r = { 0.0f, 0.0f, 721.0f, 289.0f };
	_ninePatchInfo = { r, 63, 54, (int)r.width - 665, (int)r.height - 239, NPATCH_NINE_PATCH };

	isReady = true;
}

void NPatchDemo::Update(float elasped)
{
	elapsed_time += elasped;

	float scale = (std::sin(2 * 3.14159f * 0.1f * elapsed_time) + 2) * 0.5f;

	_dest.x = (SCREEN_WIDTH - _npatchTex.width) * 0.5f;
	_dest.y = (SCREEN_HEIGHT - _npatchTex.height) * 0.5f;
	_dest.width = _npatchTex.width * scale;
	_dest.height = _npatchTex.height * scale;
}

void NPatchDemo::Draw2D()
{
	Vector2 origin = { _npatchTex.width * 0.5f, _npatchTex.height * 0.5f };
	DrawTextureNPatch(_npatchTex, _ninePatchInfo, _dest, origin, 0.0f, WHITE);
}

void NPatchDemo::Release()
{
	UnloadTexture(_npatchTex);
	isReady = false;
}
