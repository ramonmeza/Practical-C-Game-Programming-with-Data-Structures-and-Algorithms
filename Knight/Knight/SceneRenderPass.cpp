#pragma once

#include "raylib.h"
#include "rlgl.h"

#include "Knight.h"
#include "SceneRenderPass.h"

bool SceneRenderPass::Create(Scene* sc)
{
	pScene = sc;
	return true;
}

void SceneRenderPass::Render()
{
	//render background first
	vector<RenderContext>::iterator bk = pScene->_RenderQueue.Background.begin();
	while (bk != pScene->_RenderQueue.Background.end())
	{
		bk->pComponent->Draw(&Hints);
		++bk;
	}

	//render opauqe geometry from nearest to farest
	multiset<RenderContext, CompareDistanceAscending>::iterator opaque = pScene->_RenderQueue.Geometry.begin();
	while (opaque != pScene->_RenderQueue.Geometry.end())
	{
		opaque->pComponent->Draw(&Hints);
		++opaque;
	}

	//render alpha-tested geomtry from back to front
	rlSetBlendMode(RL_BLEND_ALPHA); 
	multiset<RenderContext, CompareDistanceAscending>::iterator alphatest = pScene->_RenderQueue.AlphaTest.begin();
	while (alphatest != pScene->_RenderQueue.AlphaTest.end())
	{
		alphatest->pComponent->Draw(&Hints);
		++alphatest;
	}

	//render alpha blend from back to front
	multiset<RenderContext, CompareDistanceDescending>::iterator alpha = pScene->_RenderQueue.AlphaBlending.begin();
	while (alpha != pScene->_RenderQueue.AlphaBlending.end())
	{
		rlDisableBackfaceCulling();
		rlDisableDepthMask();
		BeginBlendMode(alpha->pComponent->blendingMode);
		alpha->pComponent->Draw(&Hints);
		EndBlendMode();
		rlEnableDepthMask();
		rlEnableBackfaceCulling();
		++alpha;
	}

	//render overlay at last
	vector<RenderContext>::iterator overlay = pScene->_RenderQueue.Overlay.begin();
	while (overlay != pScene->_RenderQueue.Overlay.end())
	{
		overlay->pComponent->Draw(&Hints);
		++overlay;
	}
}

/// <summary>
/// OnAddToRender - this method is called by the SceneRenderPass to add a Component to the render queue.
/// </summary>
/// <param name="pSC">Pointer to Component object</param>
/// <param name="pSO">Pointer to SceneObject</param>
/// <returns>Return true if the Component pSC is added into renderQueue, false if it gets skipped</returns>
bool SceneRenderPass::OnAddToRender(Component* pSC, SceneObject* pSO)
{
	float dist2 = 0;

	SceneActor* pActor = dynamic_cast<SceneActor*>(pSO);

	if (pActor != nullptr) 
	{
		Vector3 pos = pActor->Position;
		if (pActiveCamera != NULL)
			dist2 = Vector3DistanceSqr(pos, pActiveCamera->GetPosition());
	}

	switch (pSC->renderQueue)
	{
		case Component::eRenderQueueType::Background:
			pScene->_RenderQueue.Background.push_back(RenderContext{ pSC, dist2 });
			break;
		case Component::eRenderQueueType::Geometry:
			pScene->_RenderQueue.Geometry.insert(RenderContext{ pSC, dist2 });
			break;
		case Component::eRenderQueueType::AlphaBlend:
			pScene->_RenderQueue.AlphaBlending.insert(RenderContext{ pSC, dist2 });
			break;
		case Component::eRenderQueueType::AlphaTest:
			pScene->_RenderQueue.AlphaTest.insert(RenderContext{ pSC, dist2 });
			break;
		case Component::eRenderQueueType::Overlay:
			pScene->_RenderQueue.Overlay.push_back(RenderContext{ pSC, dist2 });
			break;
	}

	return true;
}

void SceneRenderPass::BuildRenderQueue(SceneObject* pRoot)
{
	if (pRoot) 
	{
		if (pRoot == nullptr)
			return;

		map<Component::eComponentType, Component*>::iterator it = pRoot->_Components.begin();
		
		if (pRoot->IsActive == false)
			goto _continue;
					
		while (it != pRoot->_Components.end())
		{
			if (it->second == nullptr)
				continue;
			OnAddToRender(it->second, pRoot);
			++it;
		}

    _continue:

		if (pRoot->NextSibling)
		{
			BuildRenderQueue(pRoot->NextSibling);
		}

		if (pRoot->IsActive && pRoot->FirstChild)
		{
			BuildRenderQueue(pRoot->FirstChild);
		}
	}
}

