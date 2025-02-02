#include "SceneCamera.h"
#include "Scene.h"
#include "SceneActor.h"

#include <algorithm> // For std::lower_bound

SceneCamera::SceneCamera(Scene* Scene, const char* Name, bool IsMainCamera) : 
	SceneObject(Scene, Name)
	, _Camera({ 0 })
	, CameraMode(CAMERA_CUSTOM)
	, ShowCursor(true)
{
	_Camera.position = Vector3{ 0,1,-10.0f };
	_Camera.target = Vector3{ 0,0,0 };
	_Camera.fovy = 45.0;
	_Camera.up = Vector3{ 0,1,0 };
	if (IsMainCamera)
		_Scene->AssignMainCamera(this);
}

SceneCamera::~SceneCamera()
{
}

bool SceneCamera::Update(float ElapsedSeconds)
{
	if (!IsActive)
	{
		return false;
	}

	if (ShowCursor)
	{
		EnableCursor();
	}
	else
	{
		DisableCursor();
	}

	//let caller to set position and target if needed
	/*if (CameraMode == CAMERA_CUSTOM)
	{
		_Camera.position = Position;
		_Camera.target = Target;
	}*/

	UpdateCamera(&_Camera, CameraMode);

	return true;
}

Camera3D* SceneCamera::GetCamera3D()
{
	return &_Camera;
}





