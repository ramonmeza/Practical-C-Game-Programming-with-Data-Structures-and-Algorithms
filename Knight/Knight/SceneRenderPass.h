#pragma once

#include <set>
#include <vector>

#include "Scene.h"

typedef struct 
{
	int enabledLoc;
	int typeLoc;
	int positionLoc;
	int targetLoc;
	int colorLoc;
	int attenuationLoc;
} SceneLightShaderData;

class SceneRenderPass
{
	public:

		virtual bool Create(Scene *sc) = 0;
		virtual void Release() = 0;

		virtual void BeginScene(SceneCamera *cam = nullptr) = 0;
		virtual void Render();
		virtual void EndScene() = 0;

		virtual void BuildRenderQueue(SceneObject *pR);
		virtual bool OnAddToRender(Component* pSC, SceneObject* pSO);

		RenderHints Hints = { 0 };

		// _RenderOrder controls the order in which render passes are executed.
		int _Priority = 0;

		//CPU-side shader data for lights of the scene
		SceneLightShaderData _SceneLightData[NUM_MAX_LIGHTS] = {0};
		int ambientLoc = -1;
		int shinenessLoc = -1;
		int alphaTestLoc = -1;

	protected:

		Scene* pScene = nullptr;
		SceneCamera* pActiveCamera = nullptr;

		//Get uniform location for scene light data
		virtual void InitLightUniforms(Shader &);
		virtual void UpdateLightData(const Shader&);

		virtual void EnableAlphaTest(bool enable)
		{
			if (Hints.pOverrideShader != nullptr && alphaTestLoc >= 0)
			{
				int alphaTestValue = enable ? 1 : 0;
				SetShaderValue(*Hints.pOverrideShader, alphaTestLoc, &alphaTestValue, SHADER_UNIFORM_INT);
			}
		}
};
