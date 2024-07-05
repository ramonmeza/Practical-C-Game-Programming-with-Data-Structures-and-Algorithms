#pragma once

#include "SceneObject.h"
#include "raylib.h"
#include "raymath.h" 

class PerspectiveCamera : public SceneObject
{
public:
	
	PerspectiveCamera(Scene* Scene, const char* Name = nullptr);
	virtual ~PerspectiveCamera();
	
	CameraMode CameraMode;
	bool ShowCursor;
	Vector3 Position;
	Vector3 Target;
	float FOV_y;

	bool Update() override;

	const Camera3D* GetCamera3D();
private:
	friend class Knight;
	Camera3D _Camera;
};