#pragma once
#include "skills/Skill.h"
#include "world/World.h"
#include "layers/Layer.h"
#include "layers/MainLayer.h"
#include "stages/Play.h"

template <typename MonsterType, int monsterSize, float nestX, float nestY>
class SkillCreateMonster : public Skill
{
public:
    SkillCreateMonster(float duration, int maxCount)
        : Skill(-1, duration), maxMonsterCount_(maxCount) {}

    void tryActivate(World &world, Layer &layer, State &targetState) override
    {
        if (!owner_ || !owner_->active())
            return;
        if (active_)
            return;
        if (generatedCount >= maxMonsterCount_)
            return;

        auto &mainLayer = dynamic_cast<MainLayer &>(layer);
        auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());

        active_ = true;
        playStage.addEntityNextFrame(new MonsterType(nestX, nestY, monsterSize, monsterSize));
        generatedCount++;
    }

    void deactivate(State &targetState) override
    {
        elapsedTime_ = 0.0f;
        active_ = false;
    }

private:
    int generatedCount = 0;
    int maxMonsterCount_ = 0;
};
