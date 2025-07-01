#include "KnightUtils.h"

#include "rlgl.h"

#include <vector>
#include <unordered_map>
#include <config.h>

extern Rectangle& CenterRectangle(Rectangle& r, int width, int height)
{
	r.x = (float)(SCREEN_WIDTH - width) / 2;
	r.y = (float)(SCREEN_HEIGHT - height) / 2;
	r.width = (float)width;
	r.height = (float)height;

	return r;
}

extern float DegreesToRadians(float degrees)
{
	return degrees * (3.14159265f / 180.0f);
}

// Function to calculate the 2D bounding rectangle of a 3D cube projected by Camera3D
extern BoundingRect Get2DBoundingRectOfCube(const Vector3& cubePosition, float cubeSize, const Camera3D& camera) {
    // Define the 8 corners of the cube
    std::vector<Vector3> cubeVertices = {
        { cubePosition.x - cubeSize / 2, cubePosition.y - cubeSize / 2, cubePosition.z - cubeSize / 2 },
        { cubePosition.x + cubeSize / 2, cubePosition.y - cubeSize / 2, cubePosition.z - cubeSize / 2 },
        { cubePosition.x - cubeSize / 2, cubePosition.y + cubeSize / 2, cubePosition.z - cubeSize / 2 },
        { cubePosition.x + cubeSize / 2, cubePosition.y + cubeSize / 2, cubePosition.z - cubeSize / 2 },
        { cubePosition.x - cubeSize / 2, cubePosition.y - cubeSize / 2, cubePosition.z + cubeSize / 2 },
        { cubePosition.x + cubeSize / 2, cubePosition.y - cubeSize / 2, cubePosition.z + cubeSize / 2 },
        { cubePosition.x - cubeSize / 2, cubePosition.y + cubeSize / 2, cubePosition.z + cubeSize / 2 },
        { cubePosition.x + cubeSize / 2, cubePosition.y + cubeSize / 2, cubePosition.z + cubeSize / 2 },
    };

    // Project each 3D vertex to 2D screen space
    std::vector<Vector2> projectedPoints;
    for (const auto& vertex : cubeVertices) {
        Vector2 screenPos = GetWorldToScreen(vertex, camera);
        projectedPoints.push_back(screenPos);
    }

    // Find the bounding rectangle in 2D screen space
    float minX = projectedPoints[0].x;
    float minY = projectedPoints[0].y;
    float maxX = projectedPoints[0].x;
    float maxY = projectedPoints[0].y;

    for (const auto& point : projectedPoints) {
        if (point.x < minX) minX = point.x;
        if (point.y < minY) minY = point.y;
        if (point.x > maxX) maxX = point.x;
        if (point.y > maxY) maxY = point.y;
    }

    return BoundingRect{ { minX, minY }, { maxX, maxY } };
}

extern void RecalculateSmoothNormals(const Model& model)
{
	for (int midx = 0; midx < model.meshCount; ++midx)
	{
		std::map<Vector3, Vector3, Vector3Compare> accumulatedNormals;

		Mesh& mesh = model.meshes[midx];

		// Ensure mesh data is present
		if (mesh.normals == nullptr)
		{
			mesh.normals = (float*)MemAlloc(mesh.vertexCount * 3 * sizeof(float));
			if (mesh.normals == nullptr)
			{
				TraceLog(LOG_ERROR, "Failed to allocate memory for mesh normals");
				return; // Memory allocation failed
			}
		}

		memset(mesh.normals, 0, mesh.vertexCount * 3 * sizeof(float));

		if (mesh.indices != nullptr)
		{
			// Accumulate normals per vertex
			for (int i = 0; i < mesh.triangleCount; ++i) {
				// Retrieve vertex indices
				unsigned short idx0 = mesh.indices[i * 3];
				unsigned short idx1 = mesh.indices[i * 3 + 1];
				unsigned short idx2 = mesh.indices[i * 3 + 2];

				// Retrieve vertex positions
				Vector3 v0 = {
					mesh.vertices[idx0 * 3],
					mesh.vertices[idx0 * 3 + 1],
					mesh.vertices[idx0 * 3 + 2]
				};
				Vector3 v1 = {
					mesh.vertices[idx1 * 3],
					mesh.vertices[idx1 * 3 + 1],
					mesh.vertices[idx1 * 3 + 2]
				};
				Vector3 v2 = {
					mesh.vertices[idx2 * 3],
					mesh.vertices[idx2 * 3 + 1],
					mesh.vertices[idx2 * 3 + 2]
				};

				// Compute face normal
				Vector3 edge1 = Vector3Subtract(v1, v0);
				Vector3 edge2 = Vector3Subtract(v2, v0);
				Vector3 faceNormal = Vector3Normalize(Vector3CrossProduct(edge1, edge2));

				/* Accumulate normals for smoothing
				for (auto idx : { idx0, idx1, idx2 }) {
					mesh.normals[idx * 3] += faceNormal.x;
					mesh.normals[idx * 3 + 1] += faceNormal.y;
					mesh.normals[idx * 3 + 2] += faceNormal.z;
				}*/

				accumulatedNormals[v0] = Vector3Add(accumulatedNormals[v0], faceNormal);
				accumulatedNormals[v1] = Vector3Add(accumulatedNormals[v1], faceNormal);
				accumulatedNormals[v2] = Vector3Add(accumulatedNormals[v2], faceNormal);
			}
		}
		else
		{
			for (int i = 0; i < mesh.vertexCount; i += 3) {
				Vector3 v0 = { mesh.vertices[i * 3], mesh.vertices[i * 3 + 1], mesh.vertices[i * 3 + 2] };
				Vector3 v1 = { mesh.vertices[(i + 1) * 3], mesh.vertices[(i + 1) * 3 + 1], mesh.vertices[(i + 1) * 3 + 2] };
				Vector3 v2 = { mesh.vertices[(i + 2) * 3], mesh.vertices[(i + 2) * 3 + 1], mesh.vertices[(i + 2) * 3 + 2] };

				Vector3 faceNormal = Vector3Normalize(Vector3CrossProduct(
					Vector3Subtract(v1, v0), Vector3Subtract(v2, v0)));

				accumulatedNormals[v0] = Vector3Add(accumulatedNormals[v0], faceNormal);
				accumulatedNormals[v1] = Vector3Add(accumulatedNormals[v1], faceNormal);
				accumulatedNormals[v2] = Vector3Add(accumulatedNormals[v2], faceNormal);
			}
		}

		for (int i = 0; i < mesh.vertexCount; ++i)
		{
			Vector3 pos = { mesh.vertices[i * 3], mesh.vertices[i * 3 + 1], mesh.vertices[i * 3 + 2] };
			Vector3 normal = Vector3Normalize(accumulatedNormals[pos]);
			mesh.normals[i * 3] = normal.x;
			mesh.normals[i * 3 + 1] = normal.y;
			mesh.normals[i * 3 + 2] = normal.z;
		}

		// Update mesh GPU data
		// in raylib, mesh buffers are updated using rlUpdateVertexBuffer
		// and normal buffer is usually at index 2
		UpdateMeshBuffer(mesh, 2, mesh.normals, mesh.vertexCount * 3 * sizeof(float), 0);
	}
}

struct Vertex {
    Vector3 position;
    Vector3 normal;
    Vector2 texcoord;

    bool operator==(const Vertex& other) const {
        return Vector3Equals(position, other.position);
    }
};

struct VertexHash {
    size_t operator()(const Vertex& v) const {
        size_t hx = std::hash<float>()(v.position.x);
        size_t hy = std::hash<float>()(v.position.y);
        size_t hz = std::hash<float>()(v.position.z);
        /*size_t nx = std::hash<float>()(v.normal.x);
        size_t ny = std::hash<float>()(v.normal.y);
        size_t nz = std::hash<float>()(v.normal.z);
        size_t tx = std::hash<float>()(v.texcoord.x);
        size_t ty = std::hash<float>()(v.texcoord.y);*/

        //return hx ^ (hy << 1) ^ (hz << 2) ^ (nx << 3) ^ (ny << 4) ^ (nz << 5) ^ (tx << 6) ^ (ty << 7);
        return hx ^ (hy << 1) ^ (hz << 2);
    }
};

extern void ConvertMeshToIndexed(Mesh* mesh)
{
    std::vector<Vertex> nonIndexedVertices;

    if (mesh->indices != nullptr) {
        TraceLog(LOG_WARNING, "Mesh is already indexed. No conversion needed. Skip this action.");
		return;
    }

	TraceLog(LOG_INFO, "Converting mesh to indexed format. Current vertices = %d", mesh->vertexCount);

    // Extract existing vertex data
    for (int i = 0; i < mesh->vertexCount; i++) {
        Vertex v = {};
        v.position = {
            mesh->vertices[i * 3 + 0],
            mesh->vertices[i * 3 + 1],
            mesh->vertices[i * 3 + 2]
        };

        if (mesh->normals) {
            v.normal = {
                mesh->normals[i * 3 + 0],
                mesh->normals[i * 3 + 1],
                mesh->normals[i * 3 + 2]
            };
        }

        if (mesh->texcoords) {
            v.texcoord = {
                mesh->texcoords[i * 2 + 0],
                mesh->texcoords[i * 2 + 1]
            };
        }

        nonIndexedVertices.push_back(v);
    }

    // Build unique vertices and index buffer
    std::vector<Vertex> uniqueVertices;
    std::vector<unsigned int> indices;
    std::unordered_map<Vertex, unsigned int, VertexHash> vertexToIndex;

    for (const auto& vertex : nonIndexedVertices) {
        auto it = vertexToIndex.find(vertex);
        if (it != vertexToIndex.end()) {
            indices.push_back(it->second);
        }
        else {
            unsigned int newIndex = static_cast<unsigned int>(uniqueVertices.size());
            uniqueVertices.push_back(vertex);
            vertexToIndex[vertex] = newIndex;
            indices.push_back(newIndex);
        }
    }

    // Free old CPU vertex arrays
    MemFree(mesh->vertices);
    if (mesh->normals) MemFree(mesh->normals);
    if (mesh->texcoords) MemFree(mesh->texcoords);
    if (mesh->indices) MemFree(mesh->indices);

    // Allocate new arrays
    mesh->vertexCount = uniqueVertices.size();
    mesh->triangleCount = indices.size() / 3;

    mesh->vertices = (float*)MemAlloc(mesh->vertexCount * 3 * sizeof(float));
    mesh->normals = (float*)MemAlloc(mesh->vertexCount * 3 * sizeof(float));
    mesh->texcoords = (float*)MemAlloc(mesh->vertexCount * 2 * sizeof(float));
    mesh->indices = (unsigned short*)MemAlloc(indices.size() * sizeof(unsigned short));

    for (int i = 0; i < uniqueVertices.size(); i++) {
        mesh->vertices[i * 3 + 0] = uniqueVertices[i].position.x;
        mesh->vertices[i * 3 + 1] = uniqueVertices[i].position.y;
        mesh->vertices[i * 3 + 2] = uniqueVertices[i].position.z;

        mesh->normals[i * 3 + 0] = uniqueVertices[i].normal.x;
        mesh->normals[i * 3 + 1] = uniqueVertices[i].normal.y;
        mesh->normals[i * 3 + 2] = uniqueVertices[i].normal.z;

        mesh->texcoords[i * 2 + 0] = uniqueVertices[i].texcoord.x;
        mesh->texcoords[i * 2 + 1] = uniqueVertices[i].texcoord.y;
    }

    for (int i = 0; i < indices.size(); i++) {
        mesh->indices[i] = indices[i];
    }

    // Update counts
    mesh->triangleCount = indices.size() / 3;

	//Check if the mesh has been uploaded to GPU
	//If Yes, we need to unload the mesh buffers and re-upload the mesh with new data
    if (mesh->vaoId > 0) {

        // Unload rlgl mesh vboId data
        // raylib will check if this mesh has been uploaded to GPU and skip the upload 
        // So we just unload the buffers here and then re-upload the mesh
        // Do not use UnloadMesh() as it will free the mesh structure itself
        rlUnloadVertexArray(mesh->vaoId);
        mesh->vaoId = 0;
        if (mesh->vboId != NULL)
            for (int i = 0; i < MAX_MESH_VERTEX_BUFFERS; i++)
            {
                rlUnloadVertexBuffer(mesh->vboId[i]);
                mesh->vboId[i] = 0;
            }

        TraceLog(LOG_INFO, "Converting mesh to indexed format done. Current vertices = %d, indices=%d", mesh->vertexCount, mesh->triangleCount * 3);

        // Upload updated mesh to GPU (rebuilds VBOs/VAO)
        UploadMesh(mesh, true);

    }
}

//end of Utils.cpp