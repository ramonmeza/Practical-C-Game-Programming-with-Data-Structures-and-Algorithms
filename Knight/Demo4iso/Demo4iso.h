#pragma once

#include "Knight.h"   //Engine includes

class Demo4iso : public Knight
{
public:
	void Start() override;
	void EndGame() override;

	Demo4iso();

protected:
	PerspectiveCamera* camera;
	Texture2D _mapTiles;

	int map[10][10];

	void Update(float ElapsedSeconds) override;
	void DrawFrame() override;
	void DrawGUI() override;

};