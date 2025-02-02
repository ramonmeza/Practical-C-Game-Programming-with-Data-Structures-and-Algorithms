#pragma once

#include "SceneObject.h"
#include "raylib.h"
#include "raymath.h" 

#include "Scene.h"
#include "SceneCamera.h" 

class SceneActor : public SceneObject
{
public:

	SceneActor(Scene* Scene, const char* Name = nullptr);

	Vector3 Position;
	Vector3 Rotation;
	Vector3 Scale;

	bool AddComponent(Component* Component) override;
	bool Update(float ElapsedSeconds) override;
	
	const Matrix* GetTransformMatrix();
	const Matrix* GetRotationMatrix();
	const Matrix* GetTranslationMatrix();
	const Matrix* GetScaleMatrix();

	const Matrix* GetWorldTransformMatrix();
	Vector3 GetWorldPosition();
	Quaternion GetWorldRotation();
	Vector3 GetWorldScale();

	inline SceneCamera* GetMainCamera() { return (_Scene != NULL) ? _Scene->GetMainCameraActor() : NULL; };

protected:

	Matrix _MatTranslation;
	Matrix _MatRotation;
	Matrix _MatScale;
	Matrix _MatTransform;
	Matrix _MatWorldTransform;
};