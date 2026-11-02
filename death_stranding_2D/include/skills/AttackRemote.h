#pragma once

#include "Attack.h"
#include "skills/AttackInfo.h"
#include "structure/DoublyLinkedList.h"

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
 * - AttackInfo::RangedAttackSpeed
 */
class AttackRemote : public Attack
{
public:
    AttackRemote(int key)
        : Attack(key) {}
    virtual ~AttackRemote() = default;

    virtual AABB getAttackContainer(World &world, Layer &layer) const;
    virtual void tryHit(World &world, Layer &layer) override;
    virtual void render(World &world, Layer &layer) override;

    virtual void tryBegin(World &world, Layer &layer) override;
    virtual void end() override;

private:
    GEB::Direction direction_ = GEB::Direction::None;
    GEB::Vec2 startPos_{0, 0};
    GEB::Vec2 desPos_{0, 0};
};
