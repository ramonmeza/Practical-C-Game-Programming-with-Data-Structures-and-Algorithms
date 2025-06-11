
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

	void Update(float ElapsedSeconds) override;
	void Draw(RenderHints* pRH = nullptr) override;

	void SetColor(Color Color);
	Color GetColor();

	//void SetShader(Shader* pNewShader, int idx) override;

protected:
	Mesh _Mesh;
	Material _Material;
	Vector3 _Size;
};