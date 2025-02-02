#pragma once

#include "SceneObject.h"
#include "raylib.h"
#include "raymath.h"





class SceneCamera : public SceneObject
{
public:

	SceneCamera(Scene* Scene, const char* Name = nullptr, bool IsMainCamera = true);
	virtual ~SceneCamera();

	CameraMode CameraMode;
	bool ShowCursor;

	inline void SetPosition(Vector3 pos) { _Camera.position = pos; }
	inline void SetFovY(float fy) { _Camera.fovy = fy; }
	inline void SetLookAtPosition(Vector3 tp) { _Camera.target = tp; }
	inline void SetProjectionMode(int mode) { _Camera.projection = mode; }
	inline void SetUpward(Vector3 up) { _Camera.up = up; }

	inline const Vector3 GetPosition() { return _Camera.position; }
	inline const Vector3 GetLookAtPosition() { return _Camera.target; }
	inline const Vector3 GetUp() { return _Camera.up; }
	inline const float GetFov() { return _Camera.fovy; }
	inline const int GetProjectionMode() { return _Camera.projection; }

	bool Update(float ElapsedSeconds) override;


	Camera3D* GetCamera3D();


protected:
	friend class Knight;
	Camera3D _Camera;
};

