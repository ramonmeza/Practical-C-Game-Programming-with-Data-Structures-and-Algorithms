#pragma once

#include <vector>
#include <string>

#include "Knight.h"   //Engine includes
#include "Entities.h"  //Game includes

class Demo2c : public Knight
{
public:
	void Start() override;
	void EndGame() override;

protected:
	PerspectiveCamera* camera;

	PlayerEntity* player;
	vector<EnemyEntity> enemies;
	TerrainEntity* terrain;

	void Update(float ElapsedSeconds) override;
	void DrawFrame() override;

private:
	void InitEntities();
	void CheckDefeatEnemy();
};

