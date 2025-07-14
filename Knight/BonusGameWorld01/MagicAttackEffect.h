#pragma once

#include <vector>

#include "ParticleComponent.h"


class MagicAttackEffect : public ParticleComponent
{
public:
	bool isEnabled = false;

    MagicAttackEffect();

    void Update(float deltaTime) override;
    void Draw(RenderHints* pRH = nullptr) override;

    void Reset() override;

    void EmitParticles(float deltaTime) override;
};

//end of MagAttackEffect.h
