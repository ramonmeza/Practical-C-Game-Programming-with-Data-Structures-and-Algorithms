#include "Knight.h"

#include "rlgl.h"

bool ForwardRenderPass::Create(Scene* sc)
{
	__super::Create(sc);

	//default forward rendering pipeline use simple lighting model
	LightShader = LoadShader("../../resources/shaders/glsl330/kn_lit.vs", "../../resources/shaders/glsl330/kn_lit.fs");
	for(int i=0; i < NUM_MAX_LIGHTS; i++)
	{
		Lights[i].dirty = true;
		Lights[i].enabled = false;
		Lights[i].type = 0;  //default to directional light
		Lights[i].color = WHITE;
		Lights[i].position = Vector3{ 0.0f, 0.0f, -10.0f };
		Lights[i].target = Vector3{ 0.0f, 0.0f, 0.0f };
		Lights[i].attenuation = 0;

		Lights[i].attenuationLoc = GetShaderLocation(LightShader, TextFormat("lights[%i].enabled", i));
		Lights[i].colorLoc = GetShaderLocation(LightShader, TextFormat("lights[%i].color", i));
		Lights[i].enabledLoc = GetShaderLocation(LightShader, TextFormat("lights[%i].enabled", i));
		Lights[i].positionLoc = GetShaderLocation(LightShader, TextFormat("lights[%i].position", i));
		Lights[i].targetLoc = GetShaderLocation(LightShader, TextFormat("lights[%i].target", i));
		Lights[i].typeLoc = GetShaderLocation(LightShader, TextFormat("lights[%i].type", i));
	}

	ambientLoc = GetShaderLocation(LightShader, "ambient");

	Hints.pOverrideShader = &LightShader;

	return true;
}

void ForwardRenderPass::Release()
{
	ClearRenderQueue();
}

void ForwardRenderPass::BeginScene(SceneCamera *pOverrideCamera)
{
	UpdateLightData();

	pActiveCamera = pScene->GetMainCameraActor();
	if (pOverrideCamera != NULL)
		pActiveCamera = pOverrideCamera;
	pScene->_CurrentRenderPass = this;

	ClearRenderQueue();
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
		if (Lights[i].dirty != false)
		{
			SetShaderValue(LightShader, Lights[i].enabledLoc, &Lights[i].enabled, SHADER_UNIFORM_INT);
			if (!Lights[i].enabled) 
			{
				//if the light get disabled, there is no need to update the rest of the light data
				continue;
			}

			//Update light data
			SetShaderValue(LightShader, Lights[i].typeLoc, &Lights[i].type, SHADER_UNIFORM_INT);
			float position[3] = { Lights[i].position.x, Lights[i].position.y, Lights[i].position.z };
			SetShaderValue(LightShader, Lights[i].positionLoc, position, SHADER_UNIFORM_VEC3);
			float target[3] = { Lights[i].target.x, Lights[i].target.y, Lights[i].target.z };
			SetShaderValue(LightShader, Lights[i].targetLoc, target, SHADER_UNIFORM_VEC3);
			float color[4] = { (float)Lights[i].color.r / (float)255, (float)Lights[i].color.g / (float)255,
							   (float)Lights[i].color.b / (float)255, (float)Lights[i].color.a / (float)255 };
			SetShaderValue(LightShader, Lights[i].colorLoc, color, SHADER_UNIFORM_VEC4);
			
			//remove dirty flag
			Lights[i].dirty = false;
		}
	}

	float ambient[4] = { 0.15f, 0.15f, 0.15f, 1.0f }; //default ambient light
	SetShaderValue(LightShader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);
}

