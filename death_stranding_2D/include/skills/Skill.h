#pragma once

#include "State.h"

class ItemEntity;
class World;
class Layer;

/**
 * Skill is used to add sideEffect to character's state, not for damaging.
 */
class Skill
{
public:
    Skill(int key, float duration)
        : triggerKey_(key), duration_(duration) {}
    virtual ~Skill() = default;

    void setOwner(ItemEntity *owner) { owner_ = owner; }

    void onProcessed(float dt, State &targetState)
    {
        if (!active())
            return;

        updateElapsedTime(dt);

        if (elapsedTime() >= duration())
        {
            deactivate(targetState);
        }
    }

    virtual void tryActivate(World &world, Layer &layer, State &targetState);

    virtual void deactivate(State &targetState)
    {
        elapsedTime_ = 0.0f;
        active_ = false;
        targetState.unDo(sideEffectState_);
    }

    bool active() const { return active_; }

    float elapsedTime() const { return elapsedTime_; }
    float duration() const { return duration_; }
    void updateElapsedTime(float dt) { elapsedTime_ += dt; }
    int triggerKey() const { return triggerKey_; }

protected:
    ItemEntity *owner_ = nullptr;

    State sideEffectState_;
    float duration_ = 0.0f;
    int triggerKey_ = -1;

    float elapsedTime_ = 0.0f;
    bool active_ = false;
};
