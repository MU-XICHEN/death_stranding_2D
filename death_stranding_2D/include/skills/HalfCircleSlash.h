#pragma once

#include "skills/Attack.h"

class World;

class HalfCircleSlash : public Attack
{
public:
    using Attack::Attack;
    virtual ~HalfCircleSlash() = default;

    virtual AABB getAttackContainer(World &world) override;
    virtual bool tryHit(World &world, const ItemEntity &item, std::uintptr_t entityPtr) override;
    virtual void render(World &world) override;
};