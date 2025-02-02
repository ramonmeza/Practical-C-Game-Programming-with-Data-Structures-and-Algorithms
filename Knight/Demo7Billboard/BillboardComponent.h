#pragma once

#include "Component.h"

class BillboardComponent : public Component
{
public:

	BillboardComponent();
	~BillboardComponent();

	void Update(float ElapsedSeconds) override;
	void Draw() override;

	Texture2D texture = { 0 };
	Rectangle source = { 0 };
	Vector2 size = { 0 };
	Vector2 origin = { 0 };

	Color tint = WHITE;

	friend SceneActor;
};

//end of BillboardComponent.h