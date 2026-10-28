#pragma once

#include "skills/Attack.h"

class World;

class HalfCircleSlash : public Attack
{
public:
    HalfCircleSlash() = default;
    virtual ~HalfCircleSlash() = default;

    virtual void update(World &world) override;
    virtual void render(World &world) override;

    virtual void initialize(const AttackInfo &attackInfo) override;
};