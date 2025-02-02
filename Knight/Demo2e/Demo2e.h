#pragma once

#include <list>
#include <string>

#include "Knight.h"   //Engine includes

#include "Demo2eUI.h"

class Demo2e : public Knight
{
public:
	void Start() override;
	void EndGame() override;

protected:
	Demo2eUIManager Manager;

	void Update(float ElapsedSeconds) override;
	void DrawFrame() override;
	void DrawGUI() override;

};


