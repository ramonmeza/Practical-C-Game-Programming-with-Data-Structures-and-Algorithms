#pragma once

#include "Demo2f.h"

Entity::Entity()
{
	Actor = NULL;
}

void Entity::Update(float elaspedTime)
{
}

void TerrainEntity::Create(Scene* pScene, Entity* pParent)
{
	//Create a plan as terrain
	Actor = pScene->CreateSceneObject<SceneActor>("Terrain");
	Actor->Position = Vector3{ 0, -5, 0 };
	Actor->Scale = Vector3{ 100, 1, 100 };
	PlaneComponent* planeComponent = Actor->CreateAndAddComponent<PlaneComponent>();
	planeComponent->SetColor(DARKGREEN);
}

void Entity::DrawGUI()
{
}

