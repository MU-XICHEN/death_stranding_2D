#pragma once

#include "skills/Attack.h"

/**
 * Required properties:
 * - AttackInfo::Damage
 * - AttackInfo::Duration
 * - AttackInfo::HitStartTime
 * - AttackInfo::HitEndTime
 * - AttackInfo::HitBoxWidth
 * - AttackInfo::HitBoxHeight
 * - AttackInfo::OriginPointX
 * - AttackInfo::OriginPointY
 */
class AttackCollision : public Attack
{
public:
    using Attack::Attack;
    ~AttackCollision() = default;
    void tryBegin(World &world, Layer &layer) override;
    virtual void tryHit(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override {}
};
