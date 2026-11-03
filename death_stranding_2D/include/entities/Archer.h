#pragma once

#include "entities/Hero.h"

class Archer : public Hero
{
public:
    Archer(float x, float y, int width = ROLE_SIZE, int height = ROLE_SIZE);
    AttackInfo createAttackInfo(Attack *attack) override;

protected:
    AssetId spriteAsset() const override { return AssetId::ArcherSprite; }
};
