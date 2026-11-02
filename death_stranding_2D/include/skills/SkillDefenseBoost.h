#pragma once

#include "Skill.h"
#include "State.h"

class SkillDefenseBoost : public Skill
{
public:
    SkillDefenseBoost(int key)
        : Skill(key, 3.0f)
    {
        GEB::Pair<int, float> newProp[] = {{State::DEFENSE, 30.0f}};
        int size = sizeof(newProp) / sizeof(newProp[0]);
        sideEffectState_.initialize(newProp, size);
    }

    virtual ~SkillDefenseBoost() = default;
};