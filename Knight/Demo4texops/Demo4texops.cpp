#include "Knight.h"

#define RAYGUI_IMPLEMENTATION

#include "Demo4texops.h"

int main(int argc, char* argv[])
{
	Demo4TexOps* KnightDemo4TexOps = new Demo4TexOps();

	KnightDemo4TexOps->Start();
	KnightDemo4TexOps->GameLoop();

	delete KnightDemo4TexOps;
	return 0;
}

void Demo4TexOps::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	ShowFPS = true;

	InitEntities();
}

void Demo4TexOps::EndGame()
{
	__super::EndGame();
}

void Demo4TexOps::Update(float ElapsedSeconds)
{
	__super::Update(ElapsedSeconds);

	if (IsKeyPressed(KEY_ENTER) == true || IsMouseButtonPressed(0)== true) {
		(*it)->Release();
		++it;
		return;
	}

	if ((*it)->isReady == true)
		(*it)->Update(ElapsedSeconds);
}

void Demo4TexOps::DrawGUI()
{
	Rectangle r = {800,800,600,100};

	__super::DrawGUI();

	if (it == demos.end()) {
		ExitGameLoop();
		return;
	}

	if ((*it)->isReady == true) {
		(*it)->Draw2D();
	} else {
		// we need to create resources for this demo first
		DrawText("Loading demo ...", 600, 500, 50, WHITE);
		(*it)->Create();
	}

	GuiWindowBox(r, (*it)->title.c_str());
	GuiDrawText((*it)->description.c_str(), r, TEXT_ALIGN_CENTER, BLUE);

}

void Demo4TexOps::InitEntities()
{	
	demos.push_back(new SimpleDrawTextureDemo());
	demos.push_back(new CompressTextureDemo());
	demos.push_back(new DrawPartialRotateDemo());
	demos.push_back(new ColorBlendingDemo());
	demos.push_back(new SmoothColorBlendingDemo()); 
	demos.push_back(new SceneTransitionDemo());
	demos.push_back(new GlowDemo());
	demos.push_back(new AnimatedTexDemo());
	demos.push_back(new NPatchDemo());

	it = demos.begin();
}