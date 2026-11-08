#pragma once

#include "entities/ItemEntity.h"

class MonsterNest : public ItemEntity
{
public:
    MonsterNest(float x, float y, int width = ROLE_SIZE, int height = ROLE_SIZE);
    virtual ~MonsterNest() = default;

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;
    bool canGenerateMonsters() const;
    bool canAttackTarget(const ItemEntity *target) const override;
};
