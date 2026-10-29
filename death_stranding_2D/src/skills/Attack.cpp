#include "skills/Attack.h"
#include "entities/ItemEntity.h"
#include "stages/Play.h"
#include "layers/MainLayer.h"
#include "world/World.h"

void Attack::updateAttackInfo()
{
    if (owner_)
        attackInfo_ = owner_->createAttackInfo(this);
}

bool Attack::preTryHit(World &world, Layer &layer)
{
    if (!isActive())
        return false;

    float dt = world.gameInfo().dt();
    updateElapsedTime(dt);

    updateAttackInfo();
    const auto &atkInfo = attackInfo();

    if (elapsedTime() >= atkInfo.duration())
    {
        end();
        return false;
    }

    if (elapsedTime() < atkInfo.hitStartTime() || elapsedTime() >= atkInfo.hitEndTime())
        return false;

    return true;
}

void Attack::begin(const AttackInfo &iAtkInfo)
{
    if (active_)
        return;

    active_ = true;
    elapsedTime_ = 0.0f;
    attackInfo_ = iAtkInfo;
}

void Attack::end()
{
    hitTargets_.clear();
    active_ = false;
    elapsedTime_ = 0.0f;
}