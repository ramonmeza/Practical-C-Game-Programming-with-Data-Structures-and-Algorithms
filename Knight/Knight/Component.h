#pragma once

class SceneObject;
class SceneActor;

class Component
{
public:
	enum eComponentType
	{
		Undefined = 0,
		Model3D,
		Cube,
		Sphere
	};

public:
	Component() 
		: Type(eComponentType::Undefined)
		, _SceneObject(nullptr)
		, _SceneActor(nullptr)
	{ 
	}
	virtual ~Component() {};
	eComponentType Type;

	virtual void Update() {}
	virtual void Draw() {}

protected:
	friend class SceneObject;
	SceneObject* _SceneObject;

	friend class SceneActor;
	SceneActor* _SceneActor;
};