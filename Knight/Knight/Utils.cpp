#include "KnightUtils.h"

#include <vector>

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



//end of Utils.cpp