
#pragma once
#include "Component.h"
#include "raylib.h"

struct Model;
struct Color;

class PlaneComponent : public Component
{
public:
	float Width;
	float Height;
	int Rows;
	int Cols;

	PlaneComponent();
	~PlaneComponent();

	void Update() override;
	void Draw() override;

	void SetColor(Color Color);
	Color GetColor();

protected:
	Mesh _Mesh;
	Material _Material;
	int _Rows;
	int _Cols;
	float _Width;
	float _Height;
};