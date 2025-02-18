#pragma once

#include "rlgl.h"

#include "SceneActor.h"

class ShadowSceneLight : public SceneActor
{
	public:
		ShadowSceneLight(Scene* Scene, const char* Name = nullptr);

		bool Update(float ElapsedSeconds) override;
		virtual void SetLight(Vector3 dir, Color col);

		// Record the light matrices for future use!
		Matrix lightView = { 0 };
		Matrix lightProj = { 0 };
		Matrix lightViewProj = { 0 };
		Vector3 lightDir = { 0 };
		Color lightColor = WHITE;
		float ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
};



