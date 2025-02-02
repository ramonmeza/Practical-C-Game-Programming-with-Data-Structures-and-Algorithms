#pragma once

#include "Knight.h"   //Engine includes

class Demo5Ortho : public Knight
{
public:
	void Start() override;
	void EndGame() override;

	Demo5Ortho();

	OrthogonalCamera* OrthCam = NULL;

protected:


	void Update(float ElapsedSeconds) override;
	void DrawFrame() override;
	void DrawGUI() override;
};

