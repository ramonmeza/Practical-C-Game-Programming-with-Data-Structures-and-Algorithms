#include "Knight.h"

#include "BillboardComponent.h"
#include "rlgl.h"

BillboardComponent::BillboardComponent()
{
	renderQueue = Component::eRenderQueueType::Geometry;
	blendingMode = BLEND_ALPHA;
	EnableAlphaTest = true;
}

void BillboardComponent::Update(float ElapsedSeconds)
{	
}

void BillboardComponent::Draw(RenderHints* pRH)
{
	Vector3 billUp = { 0, 1, 0 };

	SceneCamera* pSC = this->_SceneActor->GetMainCamera();
	Camera3D* pCam = pSC->GetCamera3D();
	if (pRH != nullptr && pRH->pOverrideCamera)
		pCam = pRH->pOverrideCamera;

	if (pSC != nullptr) {

		if (AlignType == SCREEN_ALIGNED) {
			Matrix matView = rlGetMatrixModelview();
			billUp = { matView.m1, matView.m5, matView.m9 };
		}

		BeginBlendMode(blendingMode);
		if (pRH != NULL && pRH->pOverrideShader != NULL) {
			BeginShaderMode(*pRH->pOverrideShader);
			DrawBillboardPro(*pCam, texture, source, this->_SceneActor->Position, billUp, size, origin, 0, tint);
			EndShaderMode();
		}
		else 
			DrawBillboardPro(*pCam, texture, source, this->_SceneActor->Position, billUp, size, origin, 0, tint);
		EndBlendMode();
	}
}

BillboardComponent::~BillboardComponent()
{
}

//end of BillboardComponent.cpp