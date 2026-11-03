#pragma once

#include "entities/Hero.h"

class Swordsman : public Hero
{
public:
    Swordsman(float x, float y, int width = ROLE_SIZE, int height = ROLE_SIZE);
    AttackInfo createAttackInfo(Attack *attack) override;

protected:
    AssetId spriteAsset() const override { return AssetId::SwordsmanSprite; }
};
