#pragma once

#include "entities/ItemEntity.h"
#include "world/AssetManager.h"

enum class HeroType
{
    Swordsman,
    Archer,
};

struct HeroInfo
{
    float x;
    float y;
    int width;
    int height;

    GEB::Vec2 center() const { return {x + width / 2.0f, y + height / 2.0f}; }
};

class Hero : public ItemEntity
{

    friend class HeroImpl;

public:
    explicit Hero(
        float x,
        float y,
        int width,
        int height);
    virtual ~Hero() = default;

    void update(World &world, Layer &layer) override;
    void handleMovement(World &world, Layer &layer) override;
    AttackInfo createAttackInfo(Attack *attack) override = 0;
    void onTargetDefeated(const ItemEntity &target) override;
    bool onUsePotion(Potion *potion) override;

    int defeatedMonsterCount() const { return defeatedMonsterCount_; }

protected:
    AssetId spriteAsset() const override = 0;

private:
    bool checkLockDirection() const;
    int defeatedMonsterCount_ = 0;
};
