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

class HeroImpl;

class Hero : public ItemEntity
{

    friend class HeroImpl;

public:
    explicit Hero(
        float x,
        float y,
        int width,
        int height,
        float speed = PIXEL_PER_SEC);
    virtual ~Hero();

    void update(World &world, Layer &layer) override;
    void tryTiggerAttacks(World &world, Layer &layer) override;
    void handleMovement(World &world, Layer &layer) override;
    AttackInfo createAttackInfo(Attack *attack) override;

private:
    HeroInfo heroInfo_;
    HeroImpl *impl_;
};
