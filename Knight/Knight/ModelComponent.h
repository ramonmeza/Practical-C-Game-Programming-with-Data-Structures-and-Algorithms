
#pragma once
#include "Component.h"
#include "raylib.h"

#define LOAD_FLAG_COUNT  (MATERIAL_MAP_BRDF + 1)

struct Model;
struct Color;

class ModelComponent : public Component
{
protected:

	enum eLoadFlag : unsigned int
	{
		Loaded_Model = 0x01,
		Loaded_Animations = 0x02,
		DiffuseMap = 0x04,
		SpecularMap = 0x08,
		NormalMap = 0x10,
		MetalicMap = 0x20,
		RoughnessMap = 0x40,
		HeightMap = 0x80,
		CubeMap = 0x100,
		EmmissionMap = 0x200,
		OcclusionMap = 0x400
	};

public:

	ModelComponent();
	~ModelComponent();

	void Update() override;
	void Draw() override;

	void Load3DModel(const char* ModelPath,	
		const char* DiffuseMapPath = nullptr, 
		const char* SpecularMapPath = nullptr,
		const char* NormalMapPath = nullptr,
		const char* MetalicMapPath = nullptr,
		const char* RoughnessMapPath = nullptr,
		const char* HeightMapPath = nullptr,
		const char* CubeMapPath = nullptr,
		const char* EmissionMapPath = nullptr,
		const char* OcclusionMapPath = nullptr,
		Color Color = WHITE);

	bool SetAnimation(int AnimationIndex);
	int GetAnimationIndex();

protected:
	unsigned char _LoadState;
	
	Model _Model;
	ModelAnimation* _Animations;
	int _AnimationsCount;
	int _AnimationIndex;
	int _CurrentFrame;

	Texture2D _Texture2DMaps[LOAD_FLAG_COUNT];
	Color _Color;
	
	BoundingBox _BoundingBox;
};