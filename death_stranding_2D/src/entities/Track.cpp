#include "entities/Track.h"
#include "world/world.h"
#include "layers/Layer.h"

void Track::update(World &world, Layer &layer)
{
    ItemEntity::coreUpdate(world, layer);
}

void Track::tryTriggerAttacks(World &world, Layer &layer)
{

    // try begin attacks
    for (int i = 0; i < attacks_.size(); ++i)
    {
        auto *attack = attacks_[i];
        if (!attack)
            continue;
        // track does not need triggerKeys, so begin attacks directly.
        attack->begin();
    }
}