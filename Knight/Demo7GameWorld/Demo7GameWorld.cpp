#include "Demo7GameWorld.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

// Define particle count
const int maxParticles = 1000;

static TextureCubemap GenTextureCubemap(Shader shader, Texture2D panorama, int size, int format);
static Mesh GenMeshHeightmapEx(Image heightmap, Vector3 size);

int main(int argc, char* argv[])
{
	Demo7GameWorld* KnightDemo7GameWorld = new Demo7GameWorld();

	KnightDemo7GameWorld->Start();
	KnightDemo7GameWorld->GameLoop();

	delete KnightDemo7GameWorld;
	return 0;
}

Demo7GameWorld::Demo7GameWorld()
{
}

float GetMeshHeightmapValue(Mesh terrainMesh, Image heightmap, Vector3 terrainPosition, float x, float z);

void Demo7GameWorld::Start()
{
	//Initialize Knight Engine with a default scene and camera
	__super::Start();

	ShowFPS = true;

	//initialize global UI attributes
	GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

	pMainCamera = _Scene->CreateSceneObject<TopDownCamera>("Main Camera");
	pMainCamera->SetFovY(45.0f);
	pMainCamera->ShowCursor = true;

	//Place player
	Actor = _Scene->CreateSceneObject<SceneActor>("Player");
	Actor->Scale = Vector3{ 0.3f, 0.3f, 0.3f };
	Actor->Position = Vector3{ 0.f,0.5f,0.f };
	Actor->Rotation = Vector3{ 0,0,0 };
	ModelComponent* animPlayerComponent = Actor->CreateAndAddComponent<ModelComponent>();
	animPlayerComponent->Load3DModel("../../resources/models/gltf/robot.glb");
	animPlayerComponent->SetAnimation(6);
	Actor->AddComponent(animPlayerComponent);

	LookAt = Actor->Position;
	pMainCamera->SetUp(Vector3{0, 5, -5}, LookAt, 45.0f, CAMERA_PERSPECTIVE);

	image = LoadImage("../../resources/textures/heightmap.png");     // Load heightmap image (RAM)
	Image texImg = LoadImage("../../resources/textures/terrain_map.png");
	texture = LoadTextureFromImage(texImg);        // Convert image to texture (VRAM)

	mesh = GenMeshHeightmapEx(image, Vector3{ 128, 8, 128 }); // Generate heightmap mesh (RAM and VRAM)
	model = LoadModelFromMesh(mesh);                  // Load model from generated mesh	

	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture; // Set map diffuse texture
	mapPosition = { -64.0f, 0.0f, -64.0f };           // Define model position

	//UnloadImage(image);             // Unload heightmap image from RAM, already uploaded to VRAM

}

void Demo7GameWorld::EndGame()
{
	UnloadTexture(texture);     // Unload texture
	UnloadModel(model);         // Unload model

	__super::EndGame();
}

void Demo7GameWorld::Update(float ElapsedSeconds)
{
	if (IsKeyDown(KEY_W)) {
		// Move player forward based on their rotation
		Actor->Position.x += sin(DegreesToRadians(Actor->Rotation.y)) * ElapsedSeconds;
		Actor->Position.z += cos(DegreesToRadians(Actor->Rotation.y)) * ElapsedSeconds;
	}
	if (IsKeyDown(KEY_S)) {
		// Move player backward based on their rotation
		Actor->Position.x -= sin(DegreesToRadians(Actor->Rotation.y)) * ElapsedSeconds;
		Actor->Position.z -= cos(DegreesToRadians(Actor->Rotation.y)) * ElapsedSeconds;
	}
	if (IsKeyDown(KEY_A)) {
		Actor->Rotation.y += 1;  // Rotate left
	}
	if (IsKeyDown(KEY_D)) {
		Actor->Rotation.y -= 1;  // Rotate right
	}

	// Get height at cube's position on the terrain
	float terrainHeight = GetMeshHeightmapValue(mesh, image, mapPosition, Actor->Position.x, Actor->Position.z);
	Actor->Position.y = terrainHeight;

	__super::Update(ElapsedSeconds);

	Vector3 cmpos = pMainCamera->GetPosition();
	terrainHeight = GetMeshHeightmapValue(mesh, image, mapPosition, cmpos.x, cmpos.z);
	if (cmpos.y < (terrainHeight + 0.5f)) {
		cmpos.y = terrainHeight + 0.5f;
		pMainCamera->SetPosition(cmpos);
	}

	//avoid camera jittery due to sudden change of Actor's y position
	LookAt.x = Actor->Position.x;
	LookAt.z = Actor->Position.z;
	if (Actor->Position.y - LookAt.y > 0.1f) {
		LookAt.y += 1.0f * ElapsedSeconds;
	}
	if (Actor->Position.y - LookAt.y < -0.1f) {
		LookAt.y -= 1.0f * ElapsedSeconds;
	}
	pMainCamera->SetLookAtPosition(LookAt);
}

void Demo7GameWorld::DrawFrame()
{
	__super::DrawFrame();

	DrawModel(model, mapPosition, 1.0f, WHITE);

	DrawGrid(20, 1.0f);

}

void Demo7GameWorld::DrawGUI()
{
	__super::DrawGUI();
}

// Function to get height at a specific (x, z) position on the terrain
float GetMeshHeightmapValue(Mesh terrainMesh, Image heightmap, Vector3 terrainPosition, float x, float z) 
{
	// Terrain scaling dimensions based on the terrain generation
	float terrainWidth = 128.0f;
	float terrainHeight = 8.0f; // Maximum height
	float terrainDepth = 128.0f;

	// Calculate the local (x, z) position within the terrain
	//float localX = x - terrainPosition.x + terrainWidth / 2.0f;
	//float localZ = z - terrainPosition.z + terrainDepth / 2.0f;
	float localX = x - terrainPosition.x ;
	float localZ = z - terrainPosition.z ;

	// Map (localX, localZ) to heightmap pixel coordinates
	int imgX = (int)((localX / terrainWidth) * heightmap.width);
	int imgZ = (int)((localZ / terrainDepth) * heightmap.height);

	// Ensure coordinates are within heightmap bounds
	imgX = Clamp(imgX, 0, heightmap.width - 1);
	imgZ = Clamp(imgZ, 0, heightmap.height - 1);

	// Get height from the heightmap image at (imgX, imgZ)
	Color pixel = GetImageColor(heightmap, imgX, imgZ);
	float heightValue = (float)pixel.r / 255.0f; // Normalize height (assuming grayscale heightmap)

	// Scale height by the maximum terrain height
	return heightValue * terrainHeight;
}

static Mesh GenMeshHeightmapEx(Image heightmap, Vector3 size)
{
#define GRAY_VALUE(c) ((float)(c.r + c.g + c.b)/3.0f)

	Mesh mesh = { 0 };

	int mapX = heightmap.width;
	int mapZ = heightmap.height;

	Color* pixels = LoadImageColors(heightmap);

	// NOTE: One vertex per pixel
	mesh.triangleCount = (mapX - 1) * (mapZ - 1) * 2;    // One quad every four pixels

	mesh.vertexCount = mesh.triangleCount * 3;

	mesh.vertices = (float*)RL_MALLOC(mesh.vertexCount * 3 * sizeof(float));
	mesh.normals = (float*)RL_MALLOC(mesh.vertexCount * 3 * sizeof(float));
	mesh.texcoords = (float*)RL_MALLOC(mesh.vertexCount * 2 * sizeof(float));
	mesh.colors = NULL;

	int vCounter = 0;       // Used to count vertices float by float
	int tcCounter = 0;      // Used to count texcoords float by float
	int nCounter = 0;       // Used to count normals float by float

	Vector3 scaleFactor = { size.x / (mapX - 1), size.y / 255.0f, size.z / (mapZ - 1) };

	Vector3 vA = { 0 };
	Vector3 vB = { 0 };
	Vector3 vC = { 0 };
	Vector3 vN = { 0 };

	for (int z = 0; z < mapZ - 1; z++)
	{
		for (int x = 0; x < mapX - 1; x++)
		{
			// Fill vertices array with data
			//----------------------------------------------------------

			// one triangle - 3 vertex
			mesh.vertices[vCounter] = (float)x * scaleFactor.x;
			mesh.vertices[vCounter + 1] = GRAY_VALUE(pixels[x + z * mapX]) * scaleFactor.y;
			mesh.vertices[vCounter + 2] = (float)z * scaleFactor.z;

			mesh.vertices[vCounter + 3] = (float)x * scaleFactor.x;
			mesh.vertices[vCounter + 4] = GRAY_VALUE(pixels[x + (z + 1) * mapX]) * scaleFactor.y;
			mesh.vertices[vCounter + 5] = (float)(z + 1) * scaleFactor.z;

			mesh.vertices[vCounter + 6] = (float)(x + 1) * scaleFactor.x;
			mesh.vertices[vCounter + 7] = GRAY_VALUE(pixels[(x + 1) + z * mapX]) * scaleFactor.y;
			mesh.vertices[vCounter + 8] = (float)z * scaleFactor.z;

			// Another triangle - 3 vertex
			mesh.vertices[vCounter + 9] = mesh.vertices[vCounter + 6];
			mesh.vertices[vCounter + 10] = mesh.vertices[vCounter + 7];
			mesh.vertices[vCounter + 11] = mesh.vertices[vCounter + 8];

			mesh.vertices[vCounter + 12] = mesh.vertices[vCounter + 3];
			mesh.vertices[vCounter + 13] = mesh.vertices[vCounter + 4];
			mesh.vertices[vCounter + 14] = mesh.vertices[vCounter + 5];

			mesh.vertices[vCounter + 15] = (float)(x + 1) * scaleFactor.x;
			mesh.vertices[vCounter + 16] = GRAY_VALUE(pixels[(x + 1) + (z + 1) * mapX]) * scaleFactor.y;
			mesh.vertices[vCounter + 17] = (float)(z + 1) * scaleFactor.z;
			vCounter += 18;     // 6 vertex, 18 floats

			// Fill texcoords array with data
			//--------------------------------------------------------------
			mesh.texcoords[tcCounter] = (float)0;
			mesh.texcoords[tcCounter + 1] = (float)0;

			mesh.texcoords[tcCounter + 2] = (float)0;
			mesh.texcoords[tcCounter + 3] = (float)1;

			mesh.texcoords[tcCounter + 4] = (float)1;
			mesh.texcoords[tcCounter + 5] = (float)0;

			mesh.texcoords[tcCounter + 6] = mesh.texcoords[tcCounter + 4];
			mesh.texcoords[tcCounter + 7] = mesh.texcoords[tcCounter + 5];

			mesh.texcoords[tcCounter + 8] = mesh.texcoords[tcCounter + 2];
			mesh.texcoords[tcCounter + 9] = mesh.texcoords[tcCounter + 3];

			mesh.texcoords[tcCounter + 10] = (float)1;
			mesh.texcoords[tcCounter + 11] = (float)1;
			tcCounter += 12;    // 6 texcoords, 12 floats

			// Fill normals array with data
			//--------------------------------------------------------------
			for (int i = 0; i < 18; i += 9)
			{
				vA.x = mesh.vertices[nCounter + i];
				vA.y = mesh.vertices[nCounter + i + 1];
				vA.z = mesh.vertices[nCounter + i + 2];

				vB.x = mesh.vertices[nCounter + i + 3];
				vB.y = mesh.vertices[nCounter + i + 4];
				vB.z = mesh.vertices[nCounter + i + 5];

				vC.x = mesh.vertices[nCounter + i + 6];
				vC.y = mesh.vertices[nCounter + i + 7];
				vC.z = mesh.vertices[nCounter + i + 8];

				vN = Vector3Normalize(Vector3CrossProduct(Vector3Subtract(vB, vA), Vector3Subtract(vC, vA)));

				mesh.normals[nCounter + i] = vN.x;
				mesh.normals[nCounter + i + 1] = vN.y;
				mesh.normals[nCounter + i + 2] = vN.z;

				mesh.normals[nCounter + i + 3] = vN.x;
				mesh.normals[nCounter + i + 4] = vN.y;
				mesh.normals[nCounter + i + 5] = vN.z;

				mesh.normals[nCounter + i + 6] = vN.x;
				mesh.normals[nCounter + i + 7] = vN.y;
				mesh.normals[nCounter + i + 8] = vN.z;
			}

			nCounter += 18;     // 6 vertex, 18 floats
		}
	}

	UnloadImageColors(pixels);  // Unload pixels color data

	// Upload vertex data to GPU (static mesh)
	UploadMesh(&mesh, false);

	return mesh;
}
