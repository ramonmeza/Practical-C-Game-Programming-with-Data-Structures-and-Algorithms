#include "Demo2f.h"

#include <stdlib.h>

#include "raygui.h"

void EnemyEntity::Create(Scene* pScene, Entity* pParent)
{
	string name = "Enemy";
	//Place enemy
	Actor = pScene->CreateSceneObject<SceneActor>(name.c_str());
	Actor->Scale = Vector3{ 3.0f, 5.0f, 3.0f };
	Actor->Position = Vector3{ 0, 0.0f, 0.0f };
	ModelComponent* animEnemyComponent = Actor->CreateAndAddComponent<ModelComponent>();
	animEnemyComponent->Load3DModel("../../resources/models/gltf/greenman.glb");
	animEnemyComponent->SetAnimation(0);
	Actor->AddComponent(animEnemyComponent);
}

void EnemyEntity::Update(float elaspedTime)
{
	//Check if enemy can attack
	if (rechargeTime >= attackCooldown)
	{
		if (_target != NULL && _target->HP > 0)
		{  // countdown end!
			DoAttack();
		}
	} else {
		rechargeTime += elaspedTime;
	}

	//process usual entity updates
	__super::Update(elaspedTime);
}

void EnemyEntity::DrawGUI()
{
	float rate = rechargeTime / attackCooldown;
	if (rate > 1)
		rate = 1;

	GuiSetStyle(PROGRESSBAR, BASE_COLOR_PRESSED, 0xffff2f2f);
	GuiProgressBar(
		Rectangle{ 1300, 50, 400, 10 },
		Actor->GetName(), "Ready",
		&rate, 0, 1);

	float health = ((float)HP) / 100.0f;
	GuiSetStyle(PROGRESSBAR, BASE_COLOR_PRESSED, 0xff2fff2f);
	GuiProgressBar(
		Rectangle{ 1300, 30, 400, 20 },
		"0", "100",
		&health, 0, 1);
}

void EnemyEntity::Die()
{
	__super::Die();

	Demo2f::Log("Enemy dies!");
	Demo2f::GameOver(YouWin);
}



