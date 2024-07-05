#include "SphereComponent.h"
#include "SceneActor.h"

SphereComponent::SphereComponent()
	: Radius(1.0f)
	, Rings(16)
	, Slices(16)
	, _Radius(1)
	, _Rings(16)
	, _Slices(16)
{
	_Mesh = GenMeshSphere(_Radius, _Rings, _Slices);
	_Material = LoadMaterialDefault();
	_Material.maps[MATERIAL_MAP_DIFFUSE].color = LIGHTGRAY;
}

SphereComponent::~SphereComponent()
{
}

void SphereComponent::Update()
{
	__super::Update();
	if (Radius != _Radius || Rings != _Rings || Slices != _Slices)
	{
		_Radius = Radius;
		_Rings = Rings;
		_Slices = Slices;
		_Mesh = GenMeshSphere(_Radius, _Rings, _Slices);
	}
}


void SphereComponent::Draw()
{
	DrawMesh(_Mesh, _Material, *_SceneActor->GetWorldTransformMatrix());
}

void SphereComponent::SetColor(Color Color)
{
	_Material.maps[MATERIAL_MAP_DIFFUSE].color = Color;
}

Color SphereComponent::GetColor()
{
	return _Material.maps[MATERIAL_MAP_DIFFUSE].color;
}