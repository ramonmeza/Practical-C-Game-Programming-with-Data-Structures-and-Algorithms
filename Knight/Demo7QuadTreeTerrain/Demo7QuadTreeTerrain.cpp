#include "Demo7QuadTreeTerrain.h"

#include "rlgl.h" // Required for immediate mode drawing (rlVertex3f, etc.)
#include <string>   // For filename manipulation
#include <raymath.h>

const char* HEIGHTMAP_FILENAME = "../../resources/textures/heightmap.png"; // heightmap
const char* TERRAIN_TEXTURE_FILENAME = "../../resources/textures/terrain_map.png"; // terrain texture

Demo7QuadTreeTerrain::Demo7QuadTreeTerrain()
{
}

void Demo7QuadTreeTerrain::Start()
{
    //Initialize Knight Engine with a default scene and camera
    __super::Start();

    pTerrain = _Scene->CreateSceneObject<SceneActor>("Terrain");
    pTerrain->Position = Vector3{ 0.0f, 0.0f, 0.0f };           // Define model position
    pTerrain->Scale = Vector3{ 1,1,1 };
    pQuadTreeTerrain = pTerrain->CreateAndAddComponent<QuadTreeTerrainModelComponent>();
    pQuadTreeTerrain->CreateFromFile(Vector3{ 64, 13, 64 }, Vector2{ 8.0f, 8.0f }, HEIGHTMAP_FILENAME, TERRAIN_TEXTURE_FILENAME);

    pMainCamera = _Scene->CreateSceneObject<FlyThroughCamera>("Main Camera");
    pMainCamera->SetUp(pTerrain->Position, 30, 20, 20, 45, CAMERA_PERSPECTIVE);
}

void Demo7QuadTreeTerrain::Update(float elaspedTime)
{
	__super::Update(elaspedTime);   
}

void Demo7QuadTreeTerrain::DrawFrame()
{
	__super::DrawFrame();
}

void Demo7QuadTreeTerrain::DrawGUI()
{
    // Draw UI
    DrawFPS(10, 10);
    DrawText("Press B to toggle Bounding Boxes:", 10, 40, 20, YELLOW);
    DrawText("Use Mouse + arrow keys to move camera", 10, 70, 20, YELLOW);
    DrawText(TextFormat("Triangle count = %d", pQuadTreeTerrain->NumTriangles), 10, 100, 20, YELLOW);
    DrawText(TextFormat("LOD Factor: %.1f", pQuadTreeTerrain->LevelOfDetailDistance), 10, 160, 20, YELLOW);
}

void Demo7QuadTreeTerrain::EndGame()
{
}

int main(int argc, char* argv[])
{
    Demo7QuadTreeTerrain* KnightDemo7QuadTreeTerrain = new Demo7QuadTreeTerrain();
    KnightDemo7QuadTreeTerrain->Start();
    KnightDemo7QuadTreeTerrain->GameLoop();
	KnightDemo7QuadTreeTerrain->EndGame();
    delete KnightDemo7QuadTreeTerrain;
    KnightDemo7QuadTreeTerrain = nullptr;
    return 0;
}
