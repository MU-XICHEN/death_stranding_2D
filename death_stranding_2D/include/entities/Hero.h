#pragma once

#include "entities/ItemEntity.h"

struct HeroInfo
{
    float x;
    float y;
    int width;
    int height;

    GEB::Position center() const { return {x + width / 2, y + height / 2}; }
};

class Hero : public ItemEntity
{
public:
    explicit Hero()
        : ItemEntity(0, 0, ROLE_SIZE, ROLE_SIZE) {}
    using ItemEntity::ItemEntity;

    void update(World &world, Layer &layer) override;

private:
    void handleEvents(World &world, Layer &layer);

    HeroInfo heroInfo_;
};