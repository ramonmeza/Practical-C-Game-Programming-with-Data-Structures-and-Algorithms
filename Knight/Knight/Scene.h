#pragma once

#include "SceneObject.h"

class PerspectiveCamera;

class Scene
{
public:

	Scene();
	~Scene();

	SceneObject* SceneRoot;

	template<class T>
	enable_if_t<is_base_of_v<SceneObject, T>, T*> SpawnObject(const char* Name)
	{
		T* obj = new T(this);
		obj->SetName(Name);
		obj->SetParent(SceneRoot);
		return obj;
	}

	SceneObject* FindObjectByName(const char* Name, bool CaseSensitive = false);
	SceneObject* FindObjectByName(SceneObject* RootObject, const char* Name, bool CaseSensitive = false);

	bool AddSceneObject(SceneObject* Object, SceneObject* ParentObject = nullptr);
	void DestroySceneObject(SceneObject* Object);
	void DestroySceneObjectByName(const char* Name, bool CaseSensitive = false);

	void Update();
	void DrawFrame();

	PerspectiveCamera* GetMainCameraActor();

	template<class T>
	T* CreateSceneObject(const char* ObjectName, SceneObject* Parent = nullptr)
	{
		T *sceneObject = new T(this, ObjectName);
		if (!AddSceneObject(sceneObject, Parent))
		{
			delete sceneObject;
			sceneObject = nullptr;
		}
		return sceneObject;
	}
private:
	friend class PerspectiveCamera;
	PerspectiveCamera* _MainCamera;
};