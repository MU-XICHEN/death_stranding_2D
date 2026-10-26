#pragma once

#include "core/GameBase.h"
#include "structure/Set.h"
#include "entities/ItemEntity.h"

class Attack
{
public:
    void begin()
    {
        hitTargets_.clear();
        active_ = true;
    }

    void end() { active_ = false; }

    bool tryHit(unsigned int entityId)
    {
        if (!active_)
            return false;

        if (hitTargets_.contains(entityId))
            return false;

        hitTargets_.insert(entityId);
        return true;
    }

private:
    bool active_ = false;
    GEB::Structures::HashSet<unsigned int> hitTargets_;
};