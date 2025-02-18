#include "Knight.h"

#include "BillboardComponent.h"

BillboardComponent::BillboardComponent()
{
}

void BillboardComponent::Update(float ElapsedSeconds)
{
}

void BillboardComponent::Draw()
{
	SceneCamera* pSC = this->_SceneActor->GetMainCamera();
	if (pSC != NULL) {
		DrawBillboard(*pSC->GetCamera3D(), texture, this->_SceneActor->Position, this->_SceneActor->Scale.x, tint);
	}
}

BillboardComponent::~BillboardComponent()
{
}

//end of BillboardComponent.cpp