#pragma once

#include <vector>
#include <string>
#include <list>

#include "Knight.h"   //Engine includes
#include "Entities.h"  //Game includes

typedef enum {
	InProgress,
	YouWin,
	YouLose
} GameResult;

class Demo2f : public Knight
{
public:
	void Start() override;
	void EndGame() override;

	static void Log(const std::string& message);
	static void GameOver(GameResult);

protected:
	PerspectiveCamera* camera;

	PlayerEntity* player;
	EnemyEntity* enemy;
	TerrainEntity* terrain;

	void Update(float ElapsedSeconds) override;
	void DrawFrame() override;

	void DrawGUI() override;


private:
	void InitEntities();

	static std::list<std::string> messages;
	static GameResult _gameOver;

	friend EnemyEntity;
};

