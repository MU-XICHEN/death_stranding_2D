#pragma once

#include "Skill.h"
#include "State.h"

class SkillSpeedUp : public Skill
{
public:
    SkillSpeedUp(int key)
        : Skill(key, 2.0f)
    {
        GEB::Pair<int, float> newProp[] = {{State::SPEED, 100.0f}};
        int size = sizeof(newProp) / sizeof(newProp[0]);
        sideEffectState_.initialize(newProp, size);
    }

    virtual ~SkillSpeedUp() = default;
};