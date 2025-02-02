#include "Knight.h"
#include "Demo2e.h"


int main(int argc, char* argv[])
{
	Demo2e* KnightDemo2e = new Demo2e();

	KnightDemo2e->Start();
	KnightDemo2e->GameLoop();

	delete KnightDemo2e;
	return 0;
}

UIPopupManager* UIPopup::mgr = NULL;

void Demo2e::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	ShowFPS = true;

	EnableCursor();

	Manager.Create();
}

void Demo2e::EndGame()
{
	DisableCursor();

	__super::EndGame();
}

void Demo2e::Update(float ElapsedSeconds)
{
	//process input
	__super::Update(ElapsedSeconds);

	if (!Manager.IsAnyPopupShown()) {
		if (IsKeyPressed(KEY_M)) {
			Manager.Show("Menu");
		}
	}

	Manager.Update(ElapsedSeconds);
}

void Demo2e::DrawFrame()
{
	__super::DrawFrame();
}

void Demo2e::DrawGUI()
{
	Manager.Draw();
}


