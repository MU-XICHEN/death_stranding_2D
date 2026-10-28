#pragma once

#include "core/GameBase.h"
#include "structure/Set.h"
#include "entities/ItemEntity.h"
#include "entities/AABB.h"

class World;

struct AttackInfo
{
    // base
    float damage = 0.0f;
    float duration = 0.0f;
    float hitStartTime = 0.0f;
    float hitEndTime = 0.0f;
    int hitBoxWidth = 0;  // actual size of the hit box
    int hitBoxHeight = 0; // actual size of the hit box
    float angleSpeed = 0.0f;

    GEB::Vec2 originPoint = {0.0f, 0.0f};
    GEB::Direction direction = GEB::Direction::None;
};

class Attack
{
public:
    Attack(int key) : triggerKey_(key) {};
    virtual ~Attack() = default;

    virtual void render(World &world) = 0;

    virtual AABB getAttackContainer(World &world) = 0; // used for filtering items which might be attacked

    void begin(const AttackInfo &info)
    {
        if (active_)
            return;

        attackInfo_ = info;
        hitTargets_.clear();
        active_ = true;
        elapsedTime_ = 0.0f;
    }

    void end() { active_ = false; }

    virtual bool tryHit(World &world, const ItemEntity &item, std::uintptr_t entityPtr)
    {
        if (!active_)
            return false;

        if (hitTargets_.contains(entityPtr))
            return false;

        return true;
    }

    inline void updateElapsedTime(float dt) { elapsedTime_ += dt; }

    inline const AttackInfo &attackInfo() const { return attackInfo_; }

    inline const int triggerKey() const { return triggerKey_; }

    inline bool isActive() const { return active_; }

protected:
    bool active_ = false;
    float elapsedTime_ = 0.0f;

    int triggerKey_;

    AttackInfo attackInfo_;

    GEB::Structures::HashSet<std::uintptr_t> hitTargets_;
};