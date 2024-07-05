
#pragma once
#include "Component.h"
#include "raylib.h"

struct Model;
struct Color;

class CubeComponent : public Component
{
public:
	Vector3 Size;

	CubeComponent();
	~CubeComponent();

	void Update() override;
	void Draw() override;

	void SetColor(Color Color);
	Color GetColor();

protected:
	Mesh _Mesh;
	Material _Material;
	Vector3 _Size;
};