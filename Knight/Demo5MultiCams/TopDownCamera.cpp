#include "Knight.h"
#include "TopDownCamera.h"

TopDownCamera::TopDownCamera(Scene* Scene, const char* Name, bool IsMainCamera)
    : SceneCamera(Scene, Name, IsMainCamera)
{
    _Camera.projection = CAMERA_PERSPECTIVE;
}

bool TopDownCamera::Update(float ElapsedSeconds)
{
    if (!IsActive)
        return false;

    // Zoom control with mouse wheel
    cameraZoom -= GetMouseWheelMove() * zoomSpeed;
    if (cameraZoom < 5.0f) cameraZoom = 5.0f;
    if (cameraZoom > 25.0f) cameraZoom = 25.0f;

    //Calculate the camera's position 
    _Camera.position.x = _Camera.target.x - sin(mCameraHorizontalAngleShift)* cameraZoom;
    _Camera.position.z = _Camera.target.z - cos(mCameraHorizontalAngleShift)* cameraZoom;
    _Camera.position.y = cameraZoom + mCameraVerticalOffset ;

    // Pan camera with arrow keys
    if (IsKeyDown(KEY_UP)) _Camera.target.z -= cameraPanSpeed;
    if (IsKeyDown(KEY_DOWN)) _Camera.target.z += cameraPanSpeed;
    if (IsKeyDown(KEY_LEFT)) _Camera.target.x -= cameraPanSpeed;
    if (IsKeyDown(KEY_RIGHT)) _Camera.target.x += cameraPanSpeed;

    // Rotate the camera around the player when right mouse button is held
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        mCameraHorizontalAngleShift += GetMouseDelta().x * 0.01f;
        mCameraVerticalOffset += GetMouseDelta().y * 0.01f;
    }

	SceneCamera::Update(ElapsedSeconds);

    return true;
}

void TopDownCamera::SetUp(Vector3 pos, Vector3 target, float fovY, int projType)
{
    // Set up the camera with a third-person perspective	
    _Camera.position = pos; // Initial position of the camera
    _Camera.fovy = fovY; // Field of view
    _Camera.up = Vector3{ 0.0f, 1.0f, 0.0f }; // Up direction of the camera
    _Camera.target = target; // Focus on the player
    _Camera.projection = projType;
}

TopDownCamera::~TopDownCamera()
{
}
