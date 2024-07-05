#include "PerspectiveCamera.h"
#include "Scene.h"
#include "raylib.h"

PerspectiveCamera::PerspectiveCamera(Scene* Scene, const char* Name)
	: SceneObject(Scene, Name)
	, _Camera({0})
	, CameraMode(CAMERA_CUSTOM)
	, ShowCursor(true)
	, Position(Vector3{ 50, 50, 50 })
	, Target(Vector3{ 0, 10, 0 })
	, FOV_y(45)
{
	_Camera.up = Vector3 { 0.0f, 1.0f, 0.0f };
	_Camera.fovy = 45.0f;
	_Camera.projection = CAMERA_PERSPECTIVE;
	_Camera.position = Position;
	_Camera.target = Target;

	_Scene->_MainCamera = this;
}

PerspectiveCamera::~PerspectiveCamera()
{

}

bool PerspectiveCamera::Update()
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

	if (CameraMode == CAMERA_CUSTOM)
	{
		_Camera.position = Position;
		_Camera.target = Target;
	}

	UpdateCamera(&_Camera, CameraMode);
	return true;
}

const Camera3D* PerspectiveCamera::GetCamera3D()
{
	return &_Camera;
}