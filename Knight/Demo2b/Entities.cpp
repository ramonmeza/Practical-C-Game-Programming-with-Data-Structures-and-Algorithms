#pragma once

#include "Demo2b.h"

Entity::Entity()
{
	Actor = NULL;
	HP = 100;
}

void Entity::Update(float elaspedTime)
{
}

void PlayerEntity::Create(Scene* pScene, Entity* pParent)
{
	//Place player
	Actor = pScene->CreateSceneObject<SceneActor>("Player");
	Actor->Scale = Vector3{ 3.0f, 3.0f, 3.0f };
	Actor->Position.z = 30.0f;
	Actor->Rotation.y = 90.0f;
	ModelComponent* animPlayerComponent = Actor->CreateAndAddComponent<ModelComponent>();
	animPlayerComponent->Load3DModel("../../resources/models/gltf/robot.glb");
	animPlayerComponent->SetAnimation(6);
	Actor->AddComponent(animPlayerComponent);
}

EnemyEntity::EnemyEntity()
{
	respawnInterval = -1.0f;  // nagative value means not in respawn countdown
}

void EnemyEntity::Create(Scene* pScene, Entity* pParent)
{
	int x = ((rand() % 4)-2)*10;
	string name = "Enemy" + std::to_string(x);
	//Place enemy
	Actor = pScene->CreateSceneObject<SceneActor>(name.c_str());
	Actor->Scale = Vector3{ 3.0f, 5.0f, 3.0f };
	Actor->Position = Vector3{ (float)x, 0.0f, 0.0f};
	ModelComponent* animEnemyComponent = Actor->CreateAndAddComponent<ModelComponent>();
	animEnemyComponent->Load3DModel("../../resources/models/gltf/greenman.glb");
	animEnemyComponent->SetAnimation(0);
	Actor->AddComponent(animEnemyComponent);
}

void EnemyEntity::Update(float elaspedTime)
{
	if (respawnInterval > 0.0f) //Are we in respawn countdown?
	{
		respawnInterval -=  elaspedTime;
		if (respawnInterval <= 0.0f) 
		{  // countdown end!
			Resurrect();
		}
	}
}

void EnemyEntity::Die()
{
	HP = 0;
	Actor->IsActive = false;
	respawnInterval = 5.0f;
}

void EnemyEntity::Resurrect()
{
	HP = 100;
	Actor->IsActive = true;
	respawnInterval = -1.0f;
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