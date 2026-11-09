#pragma once
#include "skills/Skill.h"
#include "world/World.h"
#include "layers/Layer.h"
#include "layers/MainLayer.h"
#include "stages/Play.h"

template <typename MonsterType, int monsterSize = ROLE_SIZE>
class SkillCreateMonster : public Skill
{
public:
    // A negative maxCount keeps generating monsters without a count limit.
    SkillCreateMonster(float duration, int maxCount = -1)
        : Skill(-1, duration), maxMonsterCount_(maxCount)
    {
    }

    void tryActivate(World &world, Layer &layer, State &targetState) override
    {
        if (!owner_ || !owner_->active())
            return;
        if (active_)
            return;
        if (maxMonsterCount_ >= 0 && generatedCount >= maxMonsterCount_)
            return;

        auto &mainLayer = dynamic_cast<MainLayer &>(layer);
        auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());

        // Queue a monster below the nest and start the spawn timer.
        active_ = true;
        auto center = owner_->center();
        float x = center.x - monsterSize / 2.0f;
        float y = owner_->y() + owner_->height();
        playStage.addEntityNextFrame(new MonsterType(x, y, monsterSize, monsterSize));
        if (maxMonsterCount_ >= 0)
            generatedCount++;
    }

    void deactivate(State &targetState) override
    {
        elapsedTime_ = 0.0f;
        active_ = false;
    }

    bool finishedGenerating() const
    {
        return maxMonsterCount_ >= 0 && generatedCount >= maxMonsterCount_;
    }

private:
    int generatedCount = 0;
    int maxMonsterCount_ = 0;
};
