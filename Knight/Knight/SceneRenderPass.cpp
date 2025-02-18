#pragma once

#include "Knight.h"

#include "rlgl.h"

bool SceneRenderPass::Create(Scene* sc)
{
	pScene = sc;
	
	return true;
}

void SceneRenderPass::Render()
{
	//build render queue based on active main camera 
	ClearRenderQueue();
	BuildRenderQueue(pScene->SceneRoot);

	//render background first
	vector<RenderContext>::iterator bk = renderQueue.Background.begin();
	while (bk != renderQueue.Background.end()) {
		bk->pComponent->Draw();
		++bk;
	}

	//render opauqe geometry from nearest to farest
	multiset<RenderContext, CompareDistanceAscending>::iterator opaque = renderQueue.Geometry.begin();
	while (opaque != renderQueue.Geometry.end()) {
		opaque->pComponent->Draw();
		++opaque;
	}

	//render alpha blend
	multiset<RenderContext, CompareDistanceDescending>::iterator alpha = renderQueue.AlphaBlending.begin();
	while (alpha != renderQueue.AlphaBlending.end()) {
		rlDisableDepthMask();
		rlDisableBackfaceCulling();
		BeginBlendMode(alpha->pComponent->blendingMode);
		alpha->pComponent->Draw();
		EndBlendMode();
		rlEnableDepthMask();
		rlEnableBackfaceCulling();
		++alpha;
	}
}

void SceneRenderPass::BuildRenderQueue(SceneObject* pRoot, Shader *pShaderOverride)
{
	float dist2 = 0;

	if (pRoot) {

		map<Component::eComponentType, Component*>::iterator it = pRoot->_Components.begin();

		SceneActor* pActor = dynamic_cast<SceneActor*>(pRoot);

		if (pActor != NULL) {

			if (pActor->IsActive == false)
				goto _continue;

			Vector3 pos = pActor->Position;
			if (pActiveCamera != NULL)
				dist2 = Vector3DistanceSqr(pos, pActiveCamera->GetPosition());
		}
		
		while (it != pRoot->_Components.end())
		{
			if (it->second != nullptr && pShaderOverride != nullptr) {
				it->second->SetShader(pShaderOverride, -1);
			}

			switch (it->second->renderQueue)
			{
			case Component::eRenderQueueType::Background:
				renderQueue.Background.push_back(RenderContext{ it->second, dist2 });
				break;
			case Component::eRenderQueueType::Geometry:
				renderQueue.Geometry.insert(RenderContext{ it->second, dist2 });
				break;
			case Component::eRenderQueueType::AlphaBlend:
				renderQueue.AlphaBlending.insert(RenderContext{ it->second, dist2 });
				break;
			case Component::eRenderQueueType::Overlay:
				renderQueue.Overlay.push_back(RenderContext{ it->second, dist2 });
				break;
			}

			++it;
		}

		_continue:

		if (pRoot->NextSibling)
		{
			BuildRenderQueue(pRoot->NextSibling, pShaderOverride);
		}

		if (pRoot->IsActive && pRoot->FirstChild)
		{
			BuildRenderQueue(pRoot->FirstChild, pShaderOverride);
		}
	}
}

void SceneRenderPass::ClearRenderQueue()
{
	renderQueue.Background.clear();
	renderQueue.Geometry.clear();
	renderQueue.AlphaBlending.clear();
	renderQueue.Overlay.clear();
}
