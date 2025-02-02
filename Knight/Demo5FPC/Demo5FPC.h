#pragma once

#include "Knight.h"   //Engine includes

#define MAX_COLUMNS 20

class Demo5FPC : public Knight
{
public:
	void Start() override;

	Demo5FPC();

	PerspectiveCamera* FPSCamera;

	// Generates some random columns
	float heights[MAX_COLUMNS] = { 0 };
	Vector3 positions[MAX_COLUMNS] = { 0 };
	Color colors[MAX_COLUMNS] = { 0 };

protected:

	void DrawFrame() override;
	void DrawGUI() override;
};


