#pragma once

#include <set>
#include <vector>

#include "Scene.h"

struct RenderContext
{
	Component* pComponent = NULL;
	float distance2 = 0;
};

struct CompareDistanceAscending {
	bool operator()(const RenderContext& a, const RenderContext& b) const {
		return a.distance2 < b.distance2;
	}
};

struct CompareDistanceDescending {
	bool operator()(const RenderContext& a, const RenderContext& b) const {
		return a.distance2 > b.distance2;
	}
};

struct RenderQueues {
	vector<RenderContext> Background;
	multiset<RenderContext, CompareDistanceAscending> Geometry;
	multiset<RenderContext, CompareDistanceDescending> AlphaTest;
	multiset<RenderContext, CompareDistanceDescending> AlphaBlending;
	vector<RenderContext> Overlay;
};

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
		virtual void ClearRenderQueue();

		RenderHints Hints = { 0 };

	protected:

		RenderQueues renderQueue;
		Scene* pScene = nullptr;
		SceneCamera* pActiveCamera = nullptr;
};
