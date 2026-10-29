#pragma once

#include "State.h"
#include "entities/ItemEntity.h"
class Skill
{
public:
    Skill(int key, float duration)
        : triggerKey_(key), duration_(duration) {}
    virtual ~Skill() = default;

    void setOwner(ItemEntity *owner) { owner_ = owner; }

    void activate(State &targetState)
    {
        active_ = true;
        targetState.apply(sideEffectState_);
    }
    void deactivate(State &targetState)
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
    int triggerKey_;

    float elapsedTime_ = 0.0f;
    bool active_ = false;
};