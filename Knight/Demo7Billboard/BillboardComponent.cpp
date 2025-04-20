#include "Knight.h"

#include "BillboardComponent.h"
#include <rlgl.h>

BillboardComponent::BillboardComponent()
{
}

void BillboardComponent::Update(float ElapsedSeconds)
{	
}

void BillboardComponent::Draw()
{
	Vector3 billUp = { 0, 1, 0 };

	SceneCamera* pSC = this->_SceneActor->GetMainCamera();
	if (pSC != NULL) {

		if (AlignType == SCREEN_ALIGNED) {
			Matrix matView = rlGetMatrixModelview();
			billUp = { matView.m1, matView.m5, matView.m9 };
		}

		DrawBillboardPro(*pSC->GetCamera3D(), texture, source, this->_SceneActor->Position, billUp, size, origin, 0, tint);
	}
}

BillboardComponent::~BillboardComponent()
{
}

//end of BillboardComponent.cpp