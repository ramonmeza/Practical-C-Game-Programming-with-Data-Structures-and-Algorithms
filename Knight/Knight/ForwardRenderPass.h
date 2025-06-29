#pragma once

#include "SceneRenderPass.h"



class ForwardRenderPass : public SceneRenderPass
{
	public:

		bool Create(Scene* sc) override;
		void Release() override;

		void BeginScene(SceneCamera* cam = NULL) override;
		void EndScene() override;

		Shader LightShader = { 0 };
		int ambientLoc = -1;

		int shinenessLoc = -1;


    protected:

        void UpdateLightData();
};