#include "skills/Skill.h"
#include "entities/ItemEntity.h"
#include "world/World.h"

void Skill::tryActivate(World &world, Layer &layer, State &targetState)
{
    if (!owner_ || !owner_->active())
        return;
    if (active_)
        return;

    auto &canvas = world.canvas();
    if (!canvas.keyPressed(triggerKey_))
        return;

    // Apply the stat bonus once when the skill starts.
    active_ = true;
    targetState.apply(sideEffectState_);
}
