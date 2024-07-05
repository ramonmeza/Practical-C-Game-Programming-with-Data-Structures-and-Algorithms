#include "SceneObject.h"
#include "Scene.h"

SceneObject::SceneObject(Scene* Scene, const char* Name)
	: ID(0)
	, IsActive(true)
	, _Scene(Scene)
	, Parent(nullptr)
	, FirstChild(nullptr)
	, NextSibling(nullptr)
	, PrevSibling(nullptr)
{
	if (Name)
	{
		SetName(Name);
	}
	else
	{
		SetName("");
	}

	_Components.clear();
	_Scene->AddSceneObject(this);
}

SceneObject::~SceneObject()
{
	map<Component::eComponentType, Component*>::iterator it = _Components.begin();
	while (it != _Components.end())
	{
		delete it->second;
		++it;
	}
	_Components.clear();
	
	while (FirstChild)
	{
		SceneObject* child = FirstChild->NextSibling;
		delete FirstChild;
		FirstChild = child;
	}
}

void SceneObject::SetName(const char* Name)
{
	if (Name != nullptr)
	{
		strncpy_s(_Name, MAX_SCENE_OBJECT_NAME, Name, _TRUNCATE);
	}
}

const char* SceneObject::GetName() const
{
	return _Name;
}

void SceneObject::SetParent(SceneObject* parent)
{
	if (parent == nullptr)
	{
		return;
	}

	if (Parent && Parent->FirstChild == this)
	{
		Parent->FirstChild = NextSibling;
	}
	if (PrevSibling)
	{
		PrevSibling->NextSibling = NextSibling;
	}
	if (NextSibling)
	{
		NextSibling->PrevSibling = PrevSibling;
	}
	PrevSibling = NextSibling = nullptr;
	Parent = parent;
	if (Parent->FirstChild)
	{
		Parent->FirstChild->PrevSibling = this;
		NextSibling = Parent->FirstChild;
		Parent->FirstChild = this;
	}
	else
	{
		Parent->FirstChild = this;
	}
}


Component* SceneObject::GetComponent(Component::eComponentType ComponentType) 
{
	map<Component::eComponentType, Component*>::iterator it = _Components.find(ComponentType);
	if (_Components.end() != it)
	{
		return it->second;
	}

	return nullptr;
}

bool SceneObject::HasComponent(Component::eComponentType ComponentType)
{
	return (GetComponent(ComponentType) != nullptr);
}

bool SceneObject::AddComponent(Component* Component)
{
	if (HasComponent(Component->Type))
	{
		return false;
	}
	Component->_SceneObject = this;
	_Components[Component->Type] = Component;
	return true;
}

Component* SceneObject::RemoveComponent(Component::eComponentType ComponentType, bool destroy)
{
	Component* component = GetComponent(ComponentType);

	if (component)
	{
		_Components.erase(ComponentType);
		if (destroy)
		{
			delete component;
			component = nullptr;
		}
	}

	return component;
}

bool SceneObject::Update()
{
	if (!IsActive)
	{
		return false;
	}
	map<Component::eComponentType, Component*>::iterator it = _Components.begin();
	while (it != _Components.end())
	{
		it->second->Update();
		++it;
	}

	if (FirstChild)
	{
		FirstChild->Update();
	}

	if (NextSibling)
	{
		NextSibling->Update();
	}
	return true;
}

bool SceneObject::Draw()
{
	if (!IsActive)
	{
		return false;
	}

	map<Component::eComponentType, Component*>::iterator it = _Components.begin();
	while (it != _Components.end())
	{
		it->second->Draw();
		++it;
	}

	if (FirstChild)
	{
		FirstChild->Draw();
	}

	if (NextSibling)
	{
		NextSibling->Draw();
	}
	return true;
}