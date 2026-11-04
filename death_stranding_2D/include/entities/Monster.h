#pragma once

#include "core/GameBase.h"
#include "entities/ItemEntity.h"

class Monster : public ItemEntity
{
public:
    Monster(float x, float y, int width, int height)
        : ItemEntity(x, y, width, height) {}

    virtual ~Monster() = default;

    virtual void update(World &world, Layer &layer) override;
    virtual void handleMovement(World &world, Layer &layer) override;
    bool canAttackTarget(const ItemEntity *target) const override;

protected:
    AssetId spriteAsset() const override = 0;

    static GEB::Direction transDirVecToDiretion(const GEB::Vec2 &dirVec, GEB::Direction prevDirection);
    static inline GEB::Vec2 calculateDirVecToHero(GEB::Vec2 monsterCenter, GEB::Vec2 heroCenter)
    {
        float dx = heroCenter.x - monsterCenter.x;
        float dy = heroCenter.y - monsterCenter.y;
        return GEB::Vec2{dx, dy};
    }

    void awaken() { awakened_ = true; }
    virtual void checkDangerDistance(World &world, Layer &layer);

    bool awakened_ = false;
};

class Beast : public Monster
{
public:
    Beast(float x, float y, int width = ROLE_SIZE, int height = ROLE_SIZE);
    virtual AttackInfo createAttackInfo(Attack *attack) override;

protected:
    AssetId spriteAsset() const override { return AssetId::BeastSprite; }
};

class Ghost : public Monster
{
public:
    Ghost(float x, float y, int width = ROLE_SIZE, int height = ROLE_SIZE);
    AttackInfo createAttackInfo(Attack *attack) override;

    void render(World &world, Layer &layer) override;

protected:
    AssetId spriteAsset() const override { return AssetId::GhostSprite; }
    virtual void checkDangerDistance(World &world, Layer &layer) override;

private:
    void appear() { visible_ = true; }

    bool visible_ = false;
};
