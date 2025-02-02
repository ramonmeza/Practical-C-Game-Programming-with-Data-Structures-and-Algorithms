#include "Knight.h"

bool ForwardRenderPass::Create(Scene* sc)
{
	__super::Create(sc);

	//default forward rendering pipeline use raylib's default shader 
	return true;
}

void ForwardRenderPass::Release()
{
	ClearRenderQueue();
}

void ForwardRenderPass::BeginScene(SceneCamera *pOverrideCamera)
{
	pActiveCamera = pScene->GetMainCameraActor();
	if (pOverrideCamera != NULL)
		pActiveCamera = pOverrideCamera;
	BuildRenderQueue(pScene->SceneRoot);	
}

void ForwardRenderPass::EndScene()
{
	pActiveCamera = NULL;
}



