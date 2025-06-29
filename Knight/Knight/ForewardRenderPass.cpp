#include "Knight.h"

#include "rlgl.h"

bool ForwardRenderPass::Create(Scene* sc)
{
	__super::Create(sc);

	//default forward rendering pipeline use simple lighting model
	LightShader = LoadShader("../../resources/shaders/glsl330/kn_lit.vs", "../../resources/shaders/glsl330/kn_lit.fs");

	for(int i=0; i < NUM_MAX_LIGHTS; i++)
	{
		LightData* pLightData = &pScene->Lights[i];

		pLightData->attenuationLoc = GetShaderLocation(LightShader, TextFormat("lights[%i].enabled", i));
		pLightData->colorLoc = GetShaderLocation(LightShader, TextFormat("lights[%i].color", i));
		pLightData->enabledLoc = GetShaderLocation(LightShader, TextFormat("lights[%i].enabled", i));
		pLightData->positionLoc = GetShaderLocation(LightShader, TextFormat("lights[%i].position", i));
		pLightData->targetLoc = GetShaderLocation(LightShader, TextFormat("lights[%i].target", i));
		pLightData->typeLoc = GetShaderLocation(LightShader, TextFormat("lights[%i].type", i));
	}

	ambientLoc = GetShaderLocation(LightShader, "ambient");
	shinenessLoc = GetShaderLocation(LightShader, "materialShininess");

	//Note: rlight of Raylib use a hardcoded value of 16.0f for shininess in the 
	//this value should be overridden by the material, if not set, we provided a default value same as Raylib's rlight module 
	SetShaderValue(LightShader, shinenessLoc, &pScene->DefaultShineness, SHADER_UNIFORM_FLOAT);

	Hints.pOverrideShader = &LightShader;

	return true;
}

void ForwardRenderPass::Release()
{
}

void ForwardRenderPass::BeginScene(SceneCamera *pOverrideCamera)
{
	UpdateLightData();

	pActiveCamera = pScene->GetMainCameraActor();
	if (pOverrideCamera != NULL)
		pActiveCamera = pOverrideCamera;
	pScene->_CurrentRenderPass = this;
	pScene->ClearRenderQueue();
	BuildRenderQueue(pScene->SceneRoot);	
}

void ForwardRenderPass::EndScene()
{
	pActiveCamera = NULL;
}

void ForwardRenderPass::UpdateLightData()
{
	for(int i=0; i< NUM_MAX_LIGHTS; i++)
	{
		LightData* pLightData = &pScene->Lights[i];

		if (pLightData->dirty != false)
		{
			SetShaderValue(LightShader, pLightData->enabledLoc, &pLightData->enabled, SHADER_UNIFORM_INT);
			if (!pLightData->enabled)
			{
				//if the light get disabled, there is no need to update the rest of the light data
				continue;
			}

			//Update light data
			SetShaderValue(LightShader, pLightData->typeLoc, &pLightData->type, SHADER_UNIFORM_INT);
			float position[3] = { pLightData->position.x, pLightData->position.y, pLightData->position.z };
			SetShaderValue(LightShader, pLightData->positionLoc, position, SHADER_UNIFORM_VEC3);
			float target[3] = { pLightData->target.x, pLightData->target.y, pLightData->target.z };
			SetShaderValue(LightShader, pLightData->targetLoc, target, SHADER_UNIFORM_VEC3);
			float color[4] = { (float)pLightData->color.r / (float)255, (float)pLightData->color.g / (float)255,
							   (float)pLightData->color.b / (float)255, (float)pLightData->color.a / (float)255 };
			SetShaderValue(LightShader, pLightData->colorLoc, color, SHADER_UNIFORM_VEC4);
			
			//remove dirty flag
			pLightData->dirty = false;
		}
	}
	// Update ambient light value
	SetShaderValue(LightShader, ambientLoc, pScene->AmbientColor, SHADER_UNIFORM_VEC4);
}

