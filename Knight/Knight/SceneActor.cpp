#include "SceneActor.h"
#include "Scene.h"

SceneActor::SceneActor(Scene* Scene, const char* Name)
	: SceneObject(Scene, Name)
	, Position(Vector3{0, 0, 0})
	, Rotation(Vector3{ 0, 0, 0 })
	, Scale(Vector3{ 1, 1, 1 })
	, _MatTranslation(MatrixIdentity())
	, _MatRotation(MatrixIdentity())
	, _MatScale(MatrixIdentity())
	, _MatTransform(MatrixIdentity())
	, _MatWorldTransform(MatrixIdentity())
{

}

bool SceneActor::AddComponent(Component* Component)
{
	if (__super::AddComponent(Component))
	{
		Component->_SceneActor = this;
		return true;
	}
	return false;
}

bool SceneActor::Update(float ElapsedSeconds)
{
	if (!__super::Update(ElapsedSeconds))
	{
		return false;
	}
	
	_MatTranslation = MatrixTranslate(Position.x, Position.y, Position.z);
	_MatRotation = MatrixRotateXYZ(Vector3{ DEG2RAD * Rotation.x, DEG2RAD * Rotation.y, DEG2RAD * Rotation.z });
	_MatScale = MatrixScale(Scale.x, Scale.y, Scale.z);
	_MatTransform = MatrixMultiply(MatrixMultiply(_MatScale, _MatRotation), _MatTranslation);
	_MatWorldTransform = _MatTransform;

	SceneObject* parent = Parent;
	while (parent)
	{
		SceneActor *parentActor = dynamic_cast<SceneActor*>(parent);
		if (parentActor != nullptr)
		{
			const Matrix *parentTransform = parentActor->GetWorldTransformMatrix();
			_MatWorldTransform = MatrixMultiply(_MatTransform, *parentTransform);
			break;
		}
		else
		{
			parent = parent->Parent;
		}
	}
	return true;
}

const Matrix* SceneActor::GetTransformMatrix()
{
	return &_MatTransform;
}

const Matrix* SceneActor::GetRotationMatrix()
{
	return &_MatRotation;
}

const Matrix* SceneActor::GetTranslationMatrix()
{
	return &_MatTranslation;
}

const Matrix* SceneActor::GetScaleMatrix()
{
	return &_MatScale;
}

const Matrix* SceneActor::GetWorldTransformMatrix()
{
	return &_MatWorldTransform;
}

Vector3 SceneActor::GetWorldPosition()
{
	return Vector3 { _MatWorldTransform.m12, _MatWorldTransform.m13, _MatWorldTransform.m14 };
}

Quaternion SceneActor::GetWorldRotation()
{
	return QuaternionFromMatrix(_MatWorldTransform);
}

Vector3 SceneActor::GetWorldScale()
{
	return Vector3{ _MatWorldTransform.m0, _MatWorldTransform.m5, _MatWorldTransform.m10 };
}