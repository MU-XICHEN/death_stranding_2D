#pragma once

#include "skills/Attack.h"

class World;
class Layer;

// Implementation of the half-circle slash attack
// Rotate a hitbox in a half-circle pattern, do the attack detection each frame.
// Current issues: miss detection if FPS is low.

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
 * - AttackInfo::Direction
 */
class AttackHalfCircleSlash : public Attack
{
public:
    AttackHalfCircleSlash(int key, bool lockDirection = false)
        : Attack(key, lockDirection) {}
    virtual ~AttackHalfCircleSlash() = default;

    virtual AABB getAttackContainer(World &world, Layer &layer) const override;
    virtual void tryHit(World &world, Layer &layer) override;
    virtual void render(World &world, Layer &layer) override;
};
