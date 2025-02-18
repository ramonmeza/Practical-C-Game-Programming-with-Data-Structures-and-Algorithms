#include "Knight.h"

#include "rlgl.h"

#include "ShadowMapRenderPass.h"

ShadowMapRenderPass::ShadowMapRenderPass(ShadowSceneLight* l, int id)
{
	pLight = l;
	depthTextureId = id;
}

bool ShadowMapRenderPass::Create(Scene *sc)
{
	__super::Create(sc);

	shadowShader = LoadShader("../../resources/shaders/glsl330/shadowmap.vs", "../../resources/shaders/glsl330/shadowmap-pcf.fs");
	lightDirLoc = GetShaderLocation(shadowShader, "lightDir");
	lightColLoc = GetShaderLocation(shadowShader, "lightColor");
	shadowShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shadowShader, "viewPos");
	ambientLoc = GetShaderLocation(shadowShader, "ambient");
	lightVPLoc = GetShaderLocation(shadowShader, "lightVP");
	shadowMapLoc = GetShaderLocation(shadowShader, "shadowMap");

	Vector4 lightColorNormalized = ColorNormalize(pLight->lightColor);

	SetShaderValue(shadowShader, lightDirLoc, &pLight->lightDir, SHADER_UNIFORM_VEC3);
	SetShaderValue(shadowShader, lightColLoc, &lightColorNormalized, SHADER_UNIFORM_VEC4);
	SetShaderValue(shadowShader, ambientLoc, pLight->ambient, SHADER_UNIFORM_VEC4);
	SetShaderValue(shadowShader, GetShaderLocation(shadowShader, "shadowMapResolution"), &shadowMapResolution, SHADER_UNIFORM_INT);

	return true;
}

void ShadowMapRenderPass::Release()
{
	UnloadShader(shadowShader);
}

void ShadowMapRenderPass::BeginScene(SceneCamera* pOverrideCamera)
{
	pLight->lightViewProj = MatrixMultiply(pLight->lightView, pLight->lightProj);
	SetShaderValueMatrix(shadowShader, lightVPLoc, pLight->lightViewProj);

	//Make shadow map referenced texture as 5th texture, the first four textures are commonly used by other effects
	int slot = 4; 
	rlActiveTextureSlot(slot);
	rlEnableTexture(depthTextureId);
	rlSetUniform(shadowMapLoc, &slot, SHADER_UNIFORM_INT, 1);

	pScene->_CurrentRenderPass = this;
	pActiveCamera = pScene->GetMainCameraActor();
	if (pOverrideCamera != nullptr)
		pActiveCamera = pOverrideCamera;
	BuildRenderQueue(pScene->SceneRoot, &shadowShader);
}

void ShadowMapRenderPass::EndScene()
{
	pActiveCamera = nullptr;
	pScene->_CurrentRenderPass = nullptr;
}


