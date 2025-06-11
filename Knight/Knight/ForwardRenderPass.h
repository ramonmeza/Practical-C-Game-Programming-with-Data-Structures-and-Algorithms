#pragma once

#include "SceneRenderPass.h"

#define NUM_MAX_LIGHTS  4

typedef struct {
    bool enabled;
    int type;
    Vector3 position;
    Vector3 target;
    Color color;
    float attenuation;

    // Shader locations
    bool dirty;
    int enabledLoc;
    int typeLoc;
    int positionLoc;
    int targetLoc;
    int colorLoc;
    int attenuationLoc;
} LightData;

class ForwardRenderPass : public SceneRenderPass
{
	public:

		bool Create(Scene* sc) override;
		void Release() override;

		void BeginScene(SceneCamera* cam = NULL) override;
		void EndScene() override;

		Shader LightShader = { 0 };
		int ambientLoc = -1;
		LightData Lights[NUM_MAX_LIGHTS] = { 0 };

    protected:

        void UpdateLightData();
};