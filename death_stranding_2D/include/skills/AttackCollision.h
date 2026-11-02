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
    virtual void tryHit(World &world, Layer &layer) override;
};