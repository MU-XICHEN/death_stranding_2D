#include "skills/Attack.h"
#include "entities/ItemEntity.h"


bool Attack::tryHit(World &world, std::uintptr_t entityPtr)
{
    if (!active_)
        return false;

    if (hitTargets_.contains(entityPtr))
        return false;

    return true;
}

void Attack::updateAttackInfo()
{
    if (owner_)
        attackInfo_ = owner_->createAttackInfo(this);
}


void Attack::begin()
{
    if (active_)
        return;

    active_ = true;
    elapsedTime_ = 0.0f;
}


void Attack::end()  
{
    hitTargets_.clear();
    active_ = false;
    elapsedTime_ = 0.0f;
}