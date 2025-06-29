#pragma once

#include <set>
#include <vector>

#include "Scene.h"

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

	protected:

		Scene* pScene = nullptr;
		SceneCamera* pActiveCamera = nullptr;
};
