#pragma once

#include <vector>
#include <string>
#include <list>

#include "Knight.h"   //Engine includes
#include "Entities.h"  //Game includes

class Demo4TicTacToe : public Knight
{
public:
	void Start() override;
	void EndGame() override;

	static void Log(const std::string& message);


protected:
	PerspectiveCamera* camera;

	void Update(float ElapsedSeconds) override;
	void DrawFrame() override;

	void DrawGUI() override;


private:
	void InitEntities();
};