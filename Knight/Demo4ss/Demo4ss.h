#pragma once

#include "Knight.h"   //Engine includes

class Demo4ss : public Knight
{
public:
	void Start() override;
	void EndGame() override;

	Demo4ss();

protected:
	float scrollingBack;
	float scrollingMid;
	float scrollingFore;

	Texture2D background;
	Texture2D midground;
	Texture2D foreground;

	int map[10][10];

	void Update(float ElapsedSeconds) override;
	void DrawFrame() override;
	void DrawGUI() override;

};
