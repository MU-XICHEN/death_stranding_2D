#pragma once

#include "core/GameBase.h"
#include "structure/Set.h"
#include "entities/ItemEntity.h"

class World;

struct AttackInfo
{
    // base
    GEB::Direction direction = GEB::Direction::None;
    GEB::Vec2 originPoint = {0.0f, 0.0f};
    float damage = 0.0f;
    float duration = 0.0f;
    float hitStartTime = 0.0f;
    float hitEndTime = 0.0f;

    // for slash attacks
    float radius = 0.0f;
    float angleSpeed = 0.0f;
    int hitBoxWidth = 0;
    int hitBoxHeight = 0;
};

class Attack
{
public:
    Attack() = default;
    virtual ~Attack() = default;

    virtual void update(World &world) = 0;
    virtual void render(World &world) = 0;

    virtual void initialize(const AttackInfo &attackInfo)
    {
        attackInfo_ = attackInfo;

        elapsedTime_ = 0.0f;
        active_ = false;
    }

    void begin()
    {
        if (active_)
            return;

        hitTargets_.clear();
        active_ = true;
        elapsedTime_ = 0.0f;
    }

    void end() { active_ = false; }

    bool tryHit(unsigned int id)
    {
        if (!active_)
            return false;

        if (hitTargets_.contains(id))
            return false;

        hitTargets_.insert(id);
        return true;
    }

    inline const AttackInfo &attackInfo() const { return attackInfo_; }

protected:
    bool active_ = false;
    float elapsedTime_ = 0.0f;

    AttackInfo attackInfo_;

    GEB::Structures::HashSet<unsigned int> hitTargets_;
};