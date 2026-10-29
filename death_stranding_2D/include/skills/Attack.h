#pragma once

#include "core/GameBase.h"
#include "structure/Array.h"
#include "structure/Set.h"
#include "entities/AABB.h"
#include "skills/AttackInfo.h"

class ItemEntity;
class World;
class Layer;

/**
 * Base class for all attacks.
 * Attackers are responsible for creating AttackInfo, eg. damage, duration...
 */
class Attack
{
public:
    Attack(int key, bool lockDirection = false)
        : triggerKey_(key), lockDirection_(lockDirection) {};
    virtual ~Attack() = default;

    bool preTryHit(World &world, Layer &layer);
    virtual void tryHit(World &world, Layer &layer) = 0;
    virtual void begin(const AttackInfo &initialAttackInfo = AttackInfo{});
    virtual void end();
    virtual void render(World &world, Layer &layer) = 0;

    // attackContainer is used for filtering candidate itemlist which might handle the attack
    // The actual hit detection happens in 'virtual Attack::tryHit' method.
    virtual AABB getAttackContainer(World &world, Layer &layer) const = 0;

    void updateAttackInfo();

    inline void updateElapsedTime(float dt) { elapsedTime_ += dt; }
    inline void setOwner(ItemEntity *owner) { owner_ = owner; }
    inline AttackInfo &attackInfo() { return attackInfo_; }
    inline const AttackInfo &attackInfo() const { return attackInfo_; }
    inline const int triggerKey() const { return triggerKey_; }
    inline bool isActive() const { return active_; }
    inline float elapsedTime() const { return elapsedTime_; }
    inline bool lockDirection() const { return active_ && lockDirection_; }

protected:
    AttackInfo attackInfo_;

    bool active_ = false;
    float elapsedTime_ = 0.0f;
    int triggerKey_;
    bool lockDirection_ = false;

    GEB::Structures::HashSet<std::uintptr_t> hitTargets_;
    ItemEntity *owner_ = nullptr;
};
