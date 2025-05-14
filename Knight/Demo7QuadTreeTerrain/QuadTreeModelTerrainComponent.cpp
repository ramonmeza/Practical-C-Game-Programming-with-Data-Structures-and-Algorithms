#include "QuadTreeTerrainModelComponent.h"

QuadTreeTerrainModelComponent::QuadTreeTerrainModelComponent() 
{
    Type = Component::Model3D; // Set component type
}

QuadTreeTerrainModelComponent::~QuadTreeTerrainModelComponent()
{
    if (rootNode)
        delete rootNode; // Recursively deletes all nodes
    rootNode = nullptr;
    heightmap.clear(); // Clear heightmap data
    if (terrainTexture.id != 0) { // Unload texture if it was loaded
        UnloadTexture(terrainTexture);
    }
}

bool QuadTreeTerrainModelComponent::CreateFromFile(Vector3 terrainDimension, Vector2 texTileSize, const char* pHightmapFilePath, const char* pTerrainTexurePath)
{
	// Load heightmap data from image.also set HeightMapWidth and terrainMapHeight
    if (!LoadHeightmapFromImage(pHightmapFilePath)) {
        return false;
    }

    //update terrain scale based on speficied terrain dimension
    terrainScale.x = terrainDimension.x / HeightMapWidth; // X-axis scale
	terrainScale.z = terrainDimension.z / HeightMapDepth; // Z-axis scale
	terrainScale.y = terrainDimension.y; // Y-axis scale (from 0~1)

	//set texture tiling facotr based on the loaded heightmap
    tilingFactor.x = HeightMapWidth / texTileSize.x;  
	tilingFactor.y = HeightMapDepth / texTileSize.y;

    // Load terrain texture
    if (FileExists(pTerrainTexurePath)) {
        terrainTexture = LoadTexture(pTerrainTexurePath);
        if (terrainTexture.id != 0) {
            SetTextureFilter(terrainTexture, TEXTURE_FILTER_TRILINEAR); // Optional: for smoother look
        }
    }
    else {
        terrainTexture.id = 0; // Ensure it's marked as invalid
    }

    // Define the overall bounds of the terrain in world space, using loaded dimensions
    float worldTotalWidth = HeightMapWidth * terrainScale.x;
    float worldTotalDepth = HeightMapDepth * terrainScale.z; // Z-axis depth

    BoundingBox terrainOverallBounds = {
        { -worldTotalWidth / 2.0f, -terrainScale.y * 0.1f, -worldTotalDepth / 2.0f }, // Min corner (allow small negative Y for visual bounding box)
        { worldTotalWidth / 2.0f, terrainScale.y * 1.1f, worldTotalDepth / 2.0f }     // Max corner (allow Y slightly above max height)
    };

    // Create and build the Quadtree root node
    rootNode = new QuadTreeNode(terrainOverallBounds, 0);
    BuildQuadtreeNode(rootNode);

	return true;
}

void QuadTreeTerrainModelComponent::Update(float ElapsedSeconds)
{
	__super::Update(ElapsedSeconds); // Call base class update  

    if (IsKeyPressed(KEY_B)) {
        DebugShowBounds = !DebugShowBounds; // Toggle bounding box visibility
    }
}

void QuadTreeTerrainModelComponent::Draw()
{
	__super::Draw(); // Call base class draw

    FrustumPlane frustumPlanes[6]; // Array to hold the frustum planes
    NumTriangles = 0;
    _SceneActor->GetMainCamera()->ExtractFrustumPlanes(frustumPlanes);
    DrawQuadtreeNode(rootNode, _SceneActor->GetMainCamera(), DebugShowBounds, frustumPlanes);
}

// Get height from the global heightmap (with bounds checking)
float QuadTreeTerrainModelComponent::GetHeightmapValue(int x, int z)
{
    if (HeightMapDepth == 0 || HeightMapWidth == 0) return 0.0f; // No map loaded

    // Clamp coordinates to be within map boundaries
    if (x < 0) x = 0;
    if (x >= HeightMapWidth) x = HeightMapWidth - 1;
    if (z < 0) z = 0;
    if (z >= HeightMapDepth) z = HeightMapDepth - 1;

    return heightmap[z * HeightMapWidth + x];
}

// Load heightmap data from a grayscale image
bool QuadTreeTerrainModelComponent::LoadHeightmapFromImage(const char* fileName)
{
    if (!FileExists(fileName)) {
        return false;
    }

    Image image = LoadImage(fileName);
    if (image.data == nullptr) { // Check if image loading failed
        return false;
    }

    // Store actual dimensions from the loaded image
    HeightMapWidth = image.width;
    HeightMapDepth = image.height;
    heightmap.resize(HeightMapWidth * HeightMapDepth);

    for (int z = 0; z < HeightMapDepth; ++z) {
        for (int x = 0; x < HeightMapWidth; ++x) {
            Color pixelColor = GetImageColor(image, x, z);
            // For grayscale, R, G, and B components are usually the same.
            // We normalize the R component (0-255) to a float (0.0-1.0).
            heightmap[z * HeightMapWidth + x] = (float)pixelColor.r / 255.0f;
        }
    }

    UnloadImage(image); // Free image data from RAM
    return true;
}

// Build the Quadtree recursively
void QuadTreeTerrainModelComponent::BuildQuadtreeNode(QuadTreeNode* node) 
{
    // Stop subdividing if max depth is reached
    if (node->depth >= MaxQuadTreeDepth) {
        node->isLeaf = true;
        return;
    }

    // Calculate the size of potential children
    float halfSize = node->size / 2.0f;

    // Stop subdividing if the node's area is very small (e.g., covers less than one map pixel in extent)
    // This prevents creating too many nodes for tiny details.
    if (halfSize < terrainScale.x || halfSize < terrainScale.z) {
        node->isLeaf = true;
        return;
    }

    // If we are here, the node is not a leaf yet and can be subdivided
    node->isLeaf = false;

    // Get parent bounds
    Vector3 min = node->bounds.min;
    Vector3 max = node->bounds.max; // Y component of max is max terrain height

    // Define bounds for the four children
    // Child 0: Top-Left (NW)
    BoundingBox childBounds0 = { {min.x, min.y, min.z}, {min.x + halfSize, max.y, min.z + halfSize} };
    // Child 1: Top-Right (NE)
    BoundingBox childBounds1 = { {min.x + halfSize, min.y, min.z}, {max.x, max.y, min.z + halfSize} };
    // Child 2: Bottom-Left (SW)
    BoundingBox childBounds2 = { {min.x, min.y, min.z + halfSize}, {min.x + halfSize, max.y, max.z} };
    // Child 3: Bottom-Right (SE)
    BoundingBox childBounds3 = { {min.x + halfSize, min.y, min.z + halfSize}, {max.x, max.y, max.z} };

    node->children[0] = new QuadTreeNode(childBounds0, node->depth + 1);
    node->children[1] = new QuadTreeNode(childBounds1, node->depth + 1);
    node->children[2] = new QuadTreeNode(childBounds2, node->depth + 1);
    node->children[3] = new QuadTreeNode(childBounds3, node->depth + 1);

    // Recursively build children
    for (int i = 0; i < 4; ++i) {
        BuildQuadtreeNode(node->children[i]);
    }
}

// Draw the terrain chunk corresponding to a leaf node using immediate mode
void QuadTreeTerrainModelComponent::DrawTerrainChunk(QuadTreeNode* node)
{
    if (HeightMapWidth == 0 || HeightMapDepth == 0) return; // No map data
    if (terrainTexture.id == 0) return; // No texture loaded

    // Calculate the total dimensions of the terrain in world space
    float worldTotalWidth = HeightMapWidth * terrainScale.x;
    float worldTotalDepth = HeightMapDepth * terrainScale.z; // This is depth along Z-axis

    // Calculate the world origin (bottom-left corner of the terrain, assuming centered at 0,0)
    float worldOriginX = -worldTotalWidth / 2.0f;
    float worldOriginZ = -worldTotalDepth / 2.0f;

    // Convert node's world bounds to heightmap grid coordinates
    // Ensure we don't go outside the heightmap array
    int mapStartX = Clamp((int)roundf((node->bounds.min.x - worldOriginX) / terrainScale.x), 0, HeightMapWidth - 1);
    int mapStartZ = Clamp((int)roundf((node->bounds.min.z - worldOriginZ) / terrainScale.z), 0, HeightMapDepth - 1);
    int mapEndX = Clamp((int)roundf((node->bounds.max.x - worldOriginX) / terrainScale.x), 0, HeightMapWidth) +1;
    int mapEndZ = Clamp((int)roundf((node->bounds.max.z - worldOriginZ) / terrainScale.z), 0, HeightMapDepth) +1;

    // Ensure there's at least one quad to draw within the calculated range
    if (mapEndX <= mapStartX || mapEndZ <= mapStartZ)
        return;

    // Step determines the resolution of this chunk. For simplicity, step is 1.
    int step = 2;

    rlEnableTexture(terrainTexture.id); // Enable texturing

    rlBegin(RL_TRIANGLES); // Start drawing triangles
    rlColor4ub(255, 255, 255, 255); // Set vertex color to white to show original texture colors

    rlSetTexture(terrainTexture.id); // Bind the texture for drawing

    for (int z = mapStartZ; z < mapEndZ - step; z += step) { // Iterate through Z, stopping one step early for quad formation
        for (int x = mapStartX; x < mapEndX - step; x += step) { // Iterate through X, stopping one step early
            // Get normalized height values for the four corners of the current quad
            float h1 = GetHeightmapValue(x, z);             // Top-left
            float h2 = GetHeightmapValue(x + step, z);     // Top-right
            float h3 = GetHeightmapValue(x, z + step);     // Bottom-left
            float h4 = GetHeightmapValue(x + step, z + step); // Bottom-right

            // Calculate world coordinates for the four corners, applying scaling
            Vector3 p1 = { worldOriginX + x * terrainScale.x ,             h1 * terrainScale.y, worldOriginZ + z * terrainScale.z };
            Vector3 p2 = { worldOriginX + (x + step) * terrainScale.x, h2 * terrainScale.y, worldOriginZ + z * terrainScale.z };
            Vector3 p3 = { worldOriginX + x * terrainScale.x ,             h3 * terrainScale.y, worldOriginZ + (z + step) * terrainScale.z };
            Vector3 p4 = { worldOriginX + (x + step) * terrainScale.x, h4 * terrainScale.y, worldOriginZ + (z + step) * terrainScale.z };

            // Calculate UV coordinates for each vertex of the quad
			// These map the texture across the entire terrain, tiled by tilingFactor
            float u1 = (float)x / (HeightMapWidth - 1.0f) * tilingFactor.x;
            float v1 = (float)z / (HeightMapDepth - 1.0f) * tilingFactor.y;

            float u2 = (float)(x + step) / (HeightMapWidth - 1.0f) * tilingFactor.x;
            float v2 = v1; // (float)z / (HeightMapDepth - 1.0f) * tilingFactor.y;

            float u3 = u1; // (float)x / (HeightMapWidth - 1.0f) * tilingFacotr.x;
            float v3 = (float)(z + step) / (HeightMapWidth - 1.0f) * tilingFactor.y;

            float u4 = u2; // (float)(x + step) / (HeightMapWidth - 1.0f) * tilingFactor.x;
            float v4 = v3; // (float)(z + step) / (HeightMapDepth - 1.0f) * tilingFactor.z;


            // Triangle 1: p1, p3, p4
            Vector3 n1 = Vector3Normalize(Vector3CrossProduct(Vector3Subtract(p3, p1), Vector3Subtract(p4, p1)));
            rlNormal3f(n1.x, n1.y, n1.z);

            rlTexCoord2f(u1, v1); rlVertex3f(p1.x, p1.y, p1.z); // p1
            rlTexCoord2f(u3, v3); rlVertex3f(p3.x, p3.y, p3.z); // p3
            rlTexCoord2f(u4, v4); rlVertex3f(p4.x, p4.y, p4.z); // p4

            // Triangle 2: p1, p4, p2
            Vector3 n2 = Vector3Normalize(Vector3CrossProduct(Vector3Subtract(p4, p1), Vector3Subtract(p2, p1)));
            rlNormal3f(n2.x, n2.y, n2.z);

            rlTexCoord2f(u1, v1); rlVertex3f(p1.x, p1.y, p1.z); // p1
            rlTexCoord2f(u4, v4); rlVertex3f(p4.x, p4.y, p4.z); // p4
            rlTexCoord2f(u2, v2); rlVertex3f(p2.x, p2.y, p2.z); // p2

            NumTriangles += 2; // Increment triangle count (2 triangles per quad)
        }
    }
    rlEnd(); // Finish drawing triangles
    rlDisableTexture(); // Disable texturing
}

// Traverse the Quadtree and draw appropriate nodes/chunks
void QuadTreeTerrainModelComponent::DrawQuadtreeNode(QuadTreeNode* node, SceneCamera *pCamera, bool drawBounds, const FrustumPlane frustumPlanes[6])
{
    if (!node) return;

    // Frustum Culling: Check if the node's bounding box is visible
    if (!pCamera->IsBoundingBoxInFrustum(node->bounds, frustumPlanes)) {
        return; // Node is outside the frustum, so skip drawing it and its children
    }

	Camera3D camera = *pCamera->GetCamera3D(); // Get the camera from the SceneCamera component

    // Calculate distance from camera to node's center (on XZ plane)
    float dx = camera.position.x - node->center.x;
    float dz = camera.position.z - node->center.y; // node->center.y stores the Z-coordinate of the node's center
    float distanceToNode = sqrtf(dx * dx + dz * dz);

    // LOD Threshold: if distance is greater than node_size * factor, or if it's a leaf, or max depth draw it.
    // Otherwise, recurse into children.
    float lodThreshold = node->size * LevelOfDetailDistance;

    if (node->isLeaf || distanceToNode > lodThreshold || node->depth >= MaxQuadTreeDepth - 1) { // -1 to ensure leaves at max depth are drawn
        DrawTerrainChunk(node);

        // Optionally draw the bounding box for debugging
        if (drawBounds) {
            BoundingBox drawBox = node->bounds;
            // For visualization, we can set min/max Y of the box to actual terrain scale
            // A more accurate Y bound would involve sampling heights within this node.
            drawBox.min.y = -terrainScale.y * 0.05f; // Slightly below 0 in case of flat areas at 0 height
            drawBox.max.y = terrainScale.y * 1.05f;  // Slightly above max possible height
            Color boxColor = node->isLeaf ? LIME : RED; // Green for leaves, Red for branches
            DrawBoundingBox(drawBox, boxColor);
        }
    }
    else {
        // Recursively draw children (not a leaf and close enough to subdivide)
        for (int i = 0; i < 4; ++i) {
            if (node->children[i]) { // Check if child exists
                DrawQuadtreeNode(node->children[i], pCamera, drawBounds, frustumPlanes);
            }
        }
        // Optionally draw bounds of the parent node that got subdivided
        if (drawBounds) {
            DrawBoundingBox(node->bounds, YELLOW);
        }
    }
}
