#pragma once

#include "entities/ItemEntity.h"

struct HeroInfo
{
    float x;
    float y;
    int width;
    int height;

    GEB::Position center() const { return {x + width / 2.0f, y + height / 2.0f}; }
};

class Hero : public ItemEntity
{
public:
    explicit Hero()
        : ItemEntity(0, 0, ROLE_SIZE, ROLE_SIZE) {}
    using ItemEntity::ItemEntity;

    void update(World &world, Layer &layer) override;
    void tryTiggerSkills(World &world, Layer &layer) override;
    void handleMovement(World &world, Layer &layer) override;

private:
    template <typename T>
    AttackInfo createAttackInfo();

    HeroInfo heroInfo_;
};
