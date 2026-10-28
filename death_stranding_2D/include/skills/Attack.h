#pragma once

#include "core/GameBase.h"
#include "structure/Set.h"
#include "entities/AABB.h"

class World;
class ItemEntity;

struct AttackInfo
{
    // base
    int damage = 0;
    float duration = 0.0f;
    float hitStartTime = 0.0f;
    float hitEndTime = 0.0f;
    int hitBoxWidth = 0;  // actual size of the hit box
    int hitBoxHeight = 0; // actual size of the hit box

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

    virtual bool tryHit(World &world, std::uintptr_t entityPtr);
    virtual void begin();
    virtual void end();

    void updateAttackInfo();

    inline void updateElapsedTime(float dt) { elapsedTime_ += dt; }
    inline void setOwner(ItemEntity *owner) { owner_ = owner; }
    inline AttackInfo &attackInfo() { return attackInfo_; }
    inline const AttackInfo &attackInfo() const { return attackInfo_; }
    inline const int triggerKey() const { return triggerKey_; }
    inline bool isActive() const { return active_; }
    inline float elapsedTime() const { return elapsedTime_; }

protected:
    bool active_ = false;
    float elapsedTime_ = 0.0f;

    int triggerKey_;

    AttackInfo attackInfo_;

    GEB::Structures::HashSet<std::uintptr_t> hitTargets_;

    ItemEntity *owner_ = nullptr;
};