#include "ModelComponent.h"
#include "SceneActor.h"

ModelComponent::ModelComponent()
	: _Model({})
	, _Animations(nullptr)
	, _AnimationsCount(0)
	, _AnimationIndex(-1)
	, _CurrentFrame(-1)
	, _Texture2DMaps()
	, _Color(WHITE)
	, _LoadState(0)
	, _BoundingBox()
{
	Type = Component::eComponentType::Model3D;
}

ModelComponent::~ModelComponent()
{
	if (_LoadState & DiffuseMap)
	{
		UnloadTexture(_Texture2DMaps[MATERIAL_MAP_DIFFUSE]);
	}
	if (_LoadState & SpecularMap)
	{
		UnloadTexture(_Texture2DMaps[MATERIAL_MAP_SPECULAR]);
	}
	if (_LoadState & NormalMap)
	{
		UnloadTexture(_Texture2DMaps[MATERIAL_MAP_NORMAL]);
	}
	if (_LoadState & MetalicMap)
	{
		UnloadTexture(_Texture2DMaps[MATERIAL_MAP_METALNESS]);
	}	
	if (_LoadState & RoughnessMap)
	{
		UnloadTexture(_Texture2DMaps[MATERIAL_MAP_ROUGHNESS]);
	}	
	if (_LoadState & HeightMap)
	{
		UnloadTexture(_Texture2DMaps[MATERIAL_MAP_HEIGHT]);
	}	
	if (_LoadState & CubeMap)
	{
		UnloadTexture(_Texture2DMaps[MATERIAL_MAP_CUBEMAP]);
	}	
	if (_LoadState & EmmissionMap)
	{
		UnloadTexture(_Texture2DMaps[MATERIAL_MAP_EMISSION]);
	}
	if (_LoadState & OcclusionMap)
	{
		UnloadTexture(_Texture2DMaps[MATERIAL_MAP_OCCLUSION]);
	}

	if (_LoadState & Loaded_Animations)
	{
		UnloadModelAnimations(_Animations, _AnimationsCount);
	}
	
	if (_LoadState & Loaded_Model)
	{
		UnloadModel(_Model);
	}
}

void ModelComponent::Update()
{
	__super::Update();

	if (_SceneActor)
	{
		_Model.transform = *(_SceneActor->GetWorldTransformMatrix());
	}

	if ((_LoadState & Loaded_Animations) && _AnimationIndex >= 0 && _AnimationIndex < _AnimationsCount)
	{
		ModelAnimation anim = _Animations[_AnimationIndex];
		_CurrentFrame = (_CurrentFrame + 1) % anim.frameCount;
		UpdateModelAnimation(_Model, anim, _CurrentFrame);
	}
}

void ModelComponent::Draw()
{
	DrawModel(_Model, Vector3Zero(), 1.0f, _Color);
}

void ModelComponent::Load3DModel(const char* ModelPath,
	const char* DiffuseMapPath,
	const char* SpecularMapPath,
	const char* NormalMapPath,
	const char* MetalicMapPath,
	const char* RoughnessMapPath,
	const char* HeightMapPath,
	const char* CubeMapPath,
	const char* EmissionMapPath,
	const char* OcclusionMapPath,
	Color Color)
{
	_Model = LoadModel(ModelPath);
	_LoadState |= Loaded_Model;

	_Animations = LoadModelAnimations(ModelPath, &_AnimationsCount);
	if (_AnimationsCount > 0)
	{
		_LoadState |= Loaded_Animations;
		_AnimationIndex = 0;
		_CurrentFrame = 0;
	}

	if (DiffuseMapPath)
	{
		_Texture2DMaps[MATERIAL_MAP_DIFFUSE] = LoadTexture(DiffuseMapPath);
		_LoadState |= DiffuseMap;
		_Model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = _Texture2DMaps[MATERIAL_MAP_DIFFUSE];
	}
	if (SpecularMapPath)
	{
		_Texture2DMaps[MATERIAL_MAP_SPECULAR] = LoadTexture(SpecularMapPath);
		_LoadState |= SpecularMap;
		_Model.materials[0].maps[MATERIAL_MAP_SPECULAR].texture = _Texture2DMaps[MATERIAL_MAP_SPECULAR];
	}
	if (NormalMapPath)
	{
		_Texture2DMaps[MATERIAL_MAP_NORMAL] = LoadTexture(NormalMapPath);
		_LoadState |= NormalMap;
		_Model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = _Texture2DMaps[MATERIAL_MAP_NORMAL];
	}
	if (MetalicMapPath)
	{
		_Texture2DMaps[MATERIAL_MAP_METALNESS] = LoadTexture(MetalicMapPath);
		_LoadState |= MetalicMap;
		_Model.materials[0].maps[MATERIAL_MAP_SPECULAR].texture = _Texture2DMaps[MATERIAL_MAP_METALNESS];
	}
	if (RoughnessMapPath)
	{
		_Texture2DMaps[MATERIAL_MAP_ROUGHNESS] = LoadTexture(RoughnessMapPath);
		_LoadState |= RoughnessMap;
		_Model.materials[0].maps[MATERIAL_MAP_SPECULAR].texture = _Texture2DMaps[MATERIAL_MAP_ROUGHNESS];
	}
	if (HeightMapPath)
	{
		_Texture2DMaps[MATERIAL_MAP_HEIGHT] = LoadTexture(HeightMapPath);
		_LoadState |= HeightMap;
		_Model.materials[0].maps[MATERIAL_MAP_SPECULAR].texture = _Texture2DMaps[MATERIAL_MAP_HEIGHT];
	}
	if (EmissionMapPath)
	{
		_Texture2DMaps[MATERIAL_MAP_EMISSION] = LoadTexture(EmissionMapPath);
		_LoadState |= EmmissionMap;
		_Model.materials[0].maps[MATERIAL_MAP_SPECULAR].texture = _Texture2DMaps[MATERIAL_MAP_EMISSION];
	}
	if (OcclusionMapPath)
	{
		_Texture2DMaps[MATERIAL_MAP_OCCLUSION] = LoadTexture(OcclusionMapPath);
		_LoadState |= OcclusionMap;
		_Model.materials[0].maps[MATERIAL_MAP_SPECULAR].texture = _Texture2DMaps[MATERIAL_MAP_OCCLUSION];
	}

	_Color = Color;
	_BoundingBox = GetMeshBoundingBox(_Model.meshes[0]);
}

bool ModelComponent::SetAnimation(int AnimationIndex)
{
	if (AnimationIndex >= 0 && AnimationIndex < _AnimationsCount)
	{
		_AnimationIndex = AnimationIndex;
		_CurrentFrame = 0;
	}
	return false;
}

int ModelComponent::GetAnimationIndex()
{
	return _AnimationIndex;
}