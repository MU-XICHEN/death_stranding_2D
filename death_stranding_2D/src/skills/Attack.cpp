#include "skills/Attack.h"
#include "entities/ItemEntity.h"
#include "stages/Play.h"
#include "layers/MainLayer.h"
#include "world/World.h"

void Attack::hitTarget(ItemEntity &target, const AttackInfo &attackInfo)
{
    if (!target.active() || target.state().health() <= 0.0f)
        return;

    target.onAttacked(this, attackInfo);

    // Give the owner the kill reward if this hit defeats the target.
    if (target.state().health() <= 0.0f && owner_)
        owner_->onTargetDefeated(target);
}

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

    // Damage is allowed only during the hit time window.
    if (elapsedTime() < atkInfo.hitStartTime() || elapsedTime() >= atkInfo.hitEndTime())
        return false;

    return true;
}

void Attack::begin()
{
    if (active_)
        return;

    active_ = true;
    elapsedTime_ = 0.0f;
    updateAttackInfo();
}

void Attack::tryBegin(World &world, Layer &layer)
{
    if (active_ || !owner_)
        return;
    const auto &canvas = world.canvas();
    if (triggerKey_ >= 0 && canvas.keyPressed(triggerKey_))
    {
        begin();
    }
}

void Attack::end()
{
    if (!active_)
        return;
    // Allow these targets to be hit again by the next attack.
    hitTargets_.clear();
    active_ = false;
    elapsedTime_ = 0.0f;
}
