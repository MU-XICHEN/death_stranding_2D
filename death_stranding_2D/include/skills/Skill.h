#pragma once

#include "State.h"
class Skill
{
public:
    virtual ~Skill() = default;
    virtual void apply(State &targetState)
    {
        if (!active_)
            return;
        // todo
    }

private:
    State sideEffectState_;
    bool active_ = false;
};