#pragma once

#include "Attack.h"
#include "skills/AttackInfo.h"

class World;
class Layer;

/**
 * Required properties:
 * - AttackInfo::Damage
 * - AttackInfo::Duration
 * - AttackInfo::HitStartTime
 * - AttackInfo::HitEndTime
 * - AttackInfo::OriginPointX
 * - AttackInfo::OriginPointY
 * - AttackInfo::Direction
 */
class AttackRemote : public Attack
{
public:
    AttackRemote(int key)
        : Attack(key) {}
    virtual ~AttackRemote() = default;

    virtual AABB getAttackContainer(World &world, Layer &layer) const override;
    virtual void tryHit(World &world, Layer &layer);
    virtual void render(World &world, Layer &layer) override;

    virtual void begin(const AttackInfo &initialAttackInfo = AttackInfo{}) override
    {
        startDirection_ = initialAttackInfo.direction();
        lastPos_ = initialAttackInfo.originPoint();
    }
    virtual void end() override
    {
        Attack::end();

        startDirection_ = GEB::Direction::None;
        lastPos_ = {0, 0};
    }

private:
    GEB::Direction startDirection_ = GEB::Direction::None;
    GEB::Vec2 lastPos_{0, 0};
};