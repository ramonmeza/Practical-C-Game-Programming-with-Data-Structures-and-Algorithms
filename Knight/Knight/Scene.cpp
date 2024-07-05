
#include "Scene.h"
#include "SceneObject.h"
#include "SceneActor.h"
#include "PerspectiveCamera.h"

Scene::Scene()
	: SceneRoot(nullptr)
	, _MainCamera(nullptr)
{
	SceneRoot = new SceneObject(this);
	SceneRoot->SetName(SCENE_ROOT_NAME);
}

Scene::~Scene()
{
	if (SceneRoot)
	{
		delete SceneRoot;
		SceneRoot = nullptr;
	}
}

SceneObject* Scene::FindObjectByName(const char* Name, bool CaseSensitive)
{
	return FindObjectByName(SceneRoot, Name, CaseSensitive);
}

SceneObject* Scene::FindObjectByName(SceneObject* RootObject, const char* Name, bool CaseSensitive)
{
	SceneObject* obj = RootObject->FirstChild;
	while (obj)
	{
		int cmp = CaseSensitive ? strcmp(obj->GetName(), Name) : _strcmpi(obj->GetName(), Name);
		if (cmp == 0)
		{
			return obj;
		}
		else
		{
			if (auto foundObj = FindObjectByName(obj, Name, CaseSensitive))
			{
				return foundObj;
			}
		}
		obj = obj->NextSibling;
	}
	return nullptr;
}

bool Scene::AddSceneObject(SceneObject* Object, SceneObject* ParentObject)
{
	if (Object == nullptr)
	{
		return false;
	}
	if (ParentObject == nullptr)
	{
		Object->SetParent(SceneRoot);
	}
	else
	{
		Object->SetParent(ParentObject);
	}
	return true;
}

void Scene::DestroySceneObject(SceneObject* Object)
{
	if (Object->Parent && Object->Parent->FirstChild == Object)
	{
		Object->Parent->FirstChild = nullptr;
	}

	Object->Parent = nullptr;

	if (Object->PrevSibling)
	{
		Object->PrevSibling->NextSibling = Object->NextSibling;
	}

	if (Object->NextSibling)
	{
		Object->NextSibling->PrevSibling = Object->PrevSibling;
	}

	delete Object;
}

void Scene::DestroySceneObjectByName(const char* Name, bool CaseSensitive)
{
	SceneObject* object = FindObjectByName(Name, CaseSensitive);
	if (object)
	{
		DestroySceneObject(object);
	}
}


void Scene::Update()
{
	if (SceneRoot)
	{
		SceneRoot->Update();
	}
}

void Scene::DrawFrame()
{
	if (SceneRoot)
	{
		SceneRoot->Draw();
	}
}

PerspectiveCamera* Scene::GetMainCameraActor()
{
	return _MainCamera;
}