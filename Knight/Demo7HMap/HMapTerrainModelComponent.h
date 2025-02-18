#pragma once

#include "raylib.h"

#include "SceneActor.h"
#include "Component.h"

class HMapTerrainModelComponent : public Component
{
public:

	HMapTerrainModelComponent();
	~HMapTerrainModelComponent();

	bool CreateFromFile(Vector3 terrainDimension, Vector2 texTileSize, const char* pHightmapFilePath, const char* pTerrainTexurePath);

	void Update(float ElapsedSeconds) override;
	void Draw() override;

	Image hightMapImage;
	Model model;
	Mesh mesh;

	Color tint = WHITE;

	friend SceneActor;

protected:
	Mesh GenMeshHeightmapEx(Image heightmap, Vector3 size, Vector2 texPatchSize);
};

