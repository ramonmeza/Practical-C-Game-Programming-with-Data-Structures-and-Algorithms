#pragma once

#include "SceneRenderPass.h"

class ForwardRenderPass : public SceneRenderPass
{
	public:

		bool Create(Scene* sc) override;
		void Release() override;

		void BeginScene(SceneCamera* cam = NULL) override;
		void EndScene() override;
};