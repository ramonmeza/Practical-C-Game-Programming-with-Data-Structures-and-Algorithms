#pragma once

#include "Demo2f.h"

#include "raygui.h"

#include <stdlib.h>

void PlayerEntity::Create(Scene* pScene, Entity* pParent)
{
	//Place player
	Actor = pScene->CreateSceneObject<SceneActor>("Player");
	Actor->Scale = Vector3{ 3.0f, 3.0f, 3.0f };
	Actor->Position.z = 30.0f;
	Actor->Rotation.y = 180.0f;
	ModelComponent* animPlayerComponent = Actor->CreateAndAddComponent<ModelComponent>();
	animPlayerComponent->Load3DModel("../../resources/models/gltf/robot.glb");
	animPlayerComponent->SetAnimation(6);
	Actor->AddComponent(animPlayerComponent);
}

void PlayerEntity::Update(float elaspedTime)
{
	if (rechargeTime >= attackCooldown) 
	{
		if (IsKeyPressed(KEY_SPACE) && _target != NULL && _target->HP > 0)
			DoAttack();
	}
	else {
		rechargeTime += elaspedTime;
	}
	__super::Update(elaspedTime);
}

void PlayerEntity::DrawGUI()
{
	float rate = rechargeTime / attackCooldown;
	if (rate > 1)
		rate = 1;

	GuiSetStyle(PROGRESSBAR, BASE_COLOR_PRESSED, 0xffff2f2f);
	GuiProgressBar(
		Rectangle{ 100, 50, 400, 10 },
		Actor->GetName(), "Ready",
		&rate, 0, 1);
	float health = ((float)HP) / 100.0f;

	GuiSetStyle(PROGRESSBAR, BASE_COLOR_PRESSED, 0xff2fff2f);
	GuiProgressBar(
		Rectangle{ 100, 30, 400, 20 },
		"0", "100",
		&health, 0, 1);
}

void PlayerEntity::Die()
{
	__super::Die();

	Demo2f::Log("Player dies!");
	Demo2f::GameOver(YouLose);
}


