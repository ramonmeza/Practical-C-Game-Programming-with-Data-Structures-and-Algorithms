#include "Knight.h"
#include "FollowUpCamera.h"

FollowUpCamera::FollowUpCamera(Scene* Scene, const char* Name, bool IsMainCamera)
	: SceneCamera(Scene, Name, IsMainCamera)
{
	_Camera.projection = CAMERA_PERSPECTIVE;
}

bool FollowUpCamera::Update(float ElapsedSeconds)
{
    if (!IsActive)
        return false;

    // Adjust camera distance with mouse wheel
    if (processMouseInput)
        cameraDistance -= GetMouseWheelMove();
    if (cameraDistance < 2.0f) cameraDistance = 2.0f; // Minimum distance
    if (cameraDistance > 10.0f) cameraDistance = 10.0f; // Maximum distance

    //Calculate the camera's position based on player's rotation and distance
    _Camera.position.x = TargetActor->Position.x - sinf(mCameraHorizontalAngleShift + DegreesToRadians(TargetActor->Rotation.y)) * cameraDistance;
    _Camera.position.z = TargetActor->Position.z - cosf(mCameraHorizontalAngleShift + DegreesToRadians(TargetActor->Rotation.y)) * cameraDistance;
    _Camera.position.y = TargetActor->Position.y + 2.0f + mCameraVerticalOffset; // Keep camera above the player
    _Camera.target = TargetActor->Position; // Always focus on the player

    // Rotate the camera around the player when right mouse button is held
    if (processMouseInput && IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        mCameraHorizontalAngleShift += GetMouseDelta().x * 0.01f;
        mCameraVerticalOffset += GetMouseDelta().y * 0.01f;
    }

	Knight::Update(ElapsedSeconds);

    return true;
}

void FollowUpCamera::SetUp(SceneActor* pTarget, float fovY,  float defaultDistance, int projType)
{
    // Set up the camera with a third-person perspective	
    _Camera.position = Vector3{ 0.0f, 2.0f, -5.0f }; // Initial position of the camera
    _Camera.fovy = fovY; // Field of view
    _Camera.up = Vector3{ 0.0f, 1.0f, 0.0f }; // Up direction of the camera
    TargetActor = pTarget;
    _Camera.target = TargetActor->Position; // Focus on the player
    _Camera.projection = projType;
}

FollowUpCamera::~FollowUpCamera()
{
}
