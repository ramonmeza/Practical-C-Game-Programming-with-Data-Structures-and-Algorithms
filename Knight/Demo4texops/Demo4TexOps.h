#pragma once

#include <vector>
#include "Knight.h"   //Engine includes
#include "Entities.h"

#include "raygui.h"

class Demo4TexOps : public Knight
{
public:
	void Start() override;
	void EndGame() override;

protected:
	PerspectiveCamera* camera;

	void Update(float ElapsedSeconds) override;
	void DrawGUI() override;


private:

	vector<Entity*> demos;
	vector<Entity*>::iterator it;

	void InitEntities();
};




