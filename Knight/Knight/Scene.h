#pragma once

#include <vector>
#include <list>

#include "SceneObject.h"

class SceneCamera;
class SceneRenderer;

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

	void Update(float ElapsedSeconds);
	void DrawFrame(SceneCamera* pCamOverride = NULL);

	SceneCamera* GetMainCameraActor();

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

	inline void AssignMainCamera(SceneCamera* cam) { _MainCamera = cam; }

	SceneRenderPass* _CurrentRenderPass;

protected:
	friend class SceneCamera;
	friend class SceneObject;
	SceneCamera* _MainCamera;	

};
